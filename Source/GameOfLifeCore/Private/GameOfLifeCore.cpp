// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/GameOfLifeCore.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(LogGameOfLifeCore);

GameOfLifeCore::GameOfLifeCore(uint16 GridSizeVal) : GridSize(GridSizeVal)
{
	CurrentState = MakeUnique<GameOfLifeBoard>(GridSize);
	NextState = MakeUnique<GameOfLifeBoard>(GridSize);
}

GameOfLifeCore::~GameOfLifeCore()
{
	//Perhaps a bit paranoid but not 100% sure how well UE4 will handle garbage collection in the headless target.
	CurrentState.Reset();
	NextState.Reset();
}

bool GameOfLifeCore::LoadGameStateFromFile(FString FilePath)
{
	UE_LOG(LogGameOfLifeCore, Display, TEXT("Loading file: %s"), *FilePath);
	
	TArray<FString> FileLines;

	if(FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
	{
		if(FileLines.Num() < 1)
		{
			UE_LOG(LogGameOfLifeCore, Warning, TEXT("File was empty: %s"), *FilePath);
			return false;
		}

		if(FileLines[0] != FString("#Life 1.06"))
		{
			UE_LOG(LogGameOfLifeCore, Warning, TEXT("File was not correct format (Life 1.06) : %s"), *FilePath);
            return false;
		}
		else
		{
			FileLines.RemoveAt(0); //remove the format specifier
		}

		if(FileLines.Num() > 0)
		{
			//Empty our current cells, leaving enough slack for the cells we're about to parse in.
			LiveCells.Empty(FileLines.Num());
			
			FString StrX, StrY;
			int64 X, Y;
			
			for(int i = 0; i < FileLines.Num(); ++i)
			{
				if(FileLines[i].Split(TEXT(" "), &StrX, &StrY))
				{
					if(!ParseStringToInt64(&StrX, &X))
					{
						UE_LOG(LogGameOfLifeCore, Warning, TEXT("Failed to parse int X at line %s in file: %s"), *FString::FromInt(i + 2), *FilePath);
						return false;
					}
					else if(!ParseStringToInt64(&StrY, &Y))
					{
						UE_LOG(LogGameOfLifeCore, Warning, TEXT("Failed to parse int Y at line %s in file: %s"), *FString::FromInt(i + 2), *FilePath);
						return false;
					}
					else
					{
						LiveCells.Add(FVector2<int64>(X, Y));
					}
				}
				else
				{
					//Failed to split string, incorrect format?
					UE_LOG(LogGameOfLifeCore, Warning, TEXT("Failed to split line %s in file: %s"), *FString::FromInt(i + 2), *FilePath);
					return false;
				}
			}
		}
		else
		{
			//Loaded a valid file with no live cells.
			LiveCells.Empty();
			return true;
		}
	}
	else
	{
		UE_LOG(LogGameOfLifeCore, Warning, TEXT("Failed to load file: %s"), *FilePath);
		return false;
	}

	return true;
}

bool GameOfLifeCore::SaveGameStateToFile(FString FilePath)
{
	UE_LOG(LogGameOfLifeCore, Display, TEXT("Saving file: %s"), *FilePath);
	
	TArray<FString> FileLines;
	//Important to add the format specifier
	FileLines.Add("#Life 1.06");

	for (FVector2<int64> Cell : LiveCells)
	{
		FileLines.Add(FString::Printf(TEXT("%lld %lld"), Cell.X, Cell.Y));
	}
	
	return FFileHelper::SaveStringArrayToFile(FileLines, *FilePath);
}

void GameOfLifeCore::IterateSimulationState(uint16 Iterations, bool bPrintIterationsToLog)
{
	UE_LOG(LogGameOfLifeCore, Display, TEXT("Starting simulation for %s iterations"), *FString::FromInt(Iterations));
	CurrentState->ClearState();
	NextState->ClearState();

	//NextState will adjust it's size as necessary based upon the current state
	CurrentState->LoadStateFromLiveCellSet(&LiveCells);

	for(int i = 0; i < Iterations; ++i)
	{
		NextState->UpdateSimulationFromLastState(&CurrentState);

		//Swap our current and next states.
		GameOfLifeBoard* SwapState = CurrentState.Release();
		CurrentState.Reset(NextState.Release());
		NextState.Reset(SwapState);

		if(bPrintIterationsToLog)
		{
			LiveCells = CurrentState->GetLiveCells();
			DebugPrintGameStateToLog();
		}
	}

	LiveCells = CurrentState->GetLiveCells();
}

void GameOfLifeCore::DebugPrintGameStateToLog()
{
	/**
	 * This is obviously quite inefficient and could be done better, however it's really only meant for debug output.
	 */
	
	int64 minX = INT64_MAX, minY = INT64_MAX, maxX = INT64_MIN, maxY = INT64_MIN;

	for (FVector2<int64> Cell : LiveCells)
	{
		if(Cell.X < minX) minX = Cell.X;
		if(Cell.X > maxX) maxX = Cell.X;

		if(Cell.Y < minY) minY = Cell.Y;
		if(Cell.Y > maxY) maxY = Cell.Y;
	}
	UE_LOG(LogGameOfLifeCore, Display, TEXT(" "));
	
	if(minX != maxX && minY != maxY)
	{
		for(int y = minY; y <= maxY; y++)
		{
			FString Line = "";
		
			for(int x = minX; x <= maxX; x++)
			{
				Line += LiveCells.Contains({x, y}) ? "*" : "O";
			}
		
			UE_LOG(LogGameOfLifeCore, Display, TEXT("%s"), *Line);
		}
	}
	else
	{
		UE_LOG(LogGameOfLifeCore, Display, TEXT("*"));
	}
}

//This level of checking is super overkill, but hey
bool GameOfLifeCore::ParseStringToInt64(FString* String, int64* OutInt64)
{
	*OutInt64 = 0;
	int64 ParsedValue = 0;
	bool bIsNegative = false;
	if(String->Len() > 0)
	{
		if((*String)[0] == '-')
		{
			bIsNegative = true;
			String->RemoveAt(0); // Remove the - symbol;
		}
	}
	else
	{
		*OutInt64 = 0;
		return false;
	}

	//Not 100% sure how TCHAR interacts with this sort of ASCII value manipulation, so we get a regular Char version of the string
	char* CString = TCHAR_TO_ANSI(**String);
	for(int i = 0; i < String->Len(); i++)
	{
		char Character = CString[i];
		
		if(Character >= '0' && Character <= '9')
		{
			ParsedValue = ParsedValue * 10;
			int ToAdd = (Character - '0');

			if(ParsedValue > 0)
			{
				if(bIsNegative)
				{
					if( ToAdd > (INT64_MAX - ParsedValue) + 1 ) //INT64_MIN is kinda difficult to work around
					{
						UE_LOG(LogGameOfLifeCore, Warning, TEXT("Parsed int64 was below INT64_MIN!"));
						return false;
					}
				}
				else
				{
					if( ToAdd > INT64_MAX - ParsedValue )
					{
						UE_LOG(LogGameOfLifeCore, Warning, TEXT("Parsed int64 was too above INT64_MAX!"));
						return false;
					}
				}
			}

			ParsedValue += ToAdd;
		}
		else
		{
			UE_LOG(LogGameOfLifeCore, Warning, TEXT("Parsed int64 contained a non numerical character"));
			return false;
		}
	}
	
	if(bIsNegative) ParsedValue *= -1;
	*OutInt64 = ParsedValue;
	return true;
}
