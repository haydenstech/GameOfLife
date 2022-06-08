// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/GameOfLifeCore.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(LogGolCore);

GameOfLifeCore::GameOfLifeCore()
{	
}

GameOfLifeCore::~GameOfLifeCore()
{
}

bool GameOfLifeCore::LoadGameStateFromFile(FString FilePath)
{
	UE_LOG(LogGolCore, Display, TEXT("Loading file: %s"), *FilePath);
	
	TArray<FString> FileLines;

	if(FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
	{
		if(FileLines.Num() < 1)
		{
			UE_LOG(LogGolCore, Warning, TEXT("File was empty: %s"), *FilePath);
			return false;
		}

		if(FileLines[0] != FString("#Life 1.06"))
		{
			UE_LOG(LogGolCore, Warning, TEXT("File was not correct format (Life 1.06) : %s"), *FilePath);
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
					if(ParseStringToInt64(&StrX, &X) && ParseStringToInt64(&StrY, &Y))
					{
						LiveCells.Add(FVector2<int64>(X, Y));
					}
					else
					{
						//Failed to parse one of the strings, +2 to account for removed initial line + index from 0
						UE_LOG(LogGolCore, Warning, TEXT("Failed to parse int at line %s in file: %s"), *FString::FromInt(i + 2), *FilePath);
						return false;
					}
				}
				else
				{
					//Failed to split string, incorrect format?
					UE_LOG(LogGolCore, Warning, TEXT("Failed to split line %s in file: %s"), *FString::FromInt(i + 2), *FilePath);
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
		UE_LOG(LogGolCore, Warning, TEXT("Failed to load file: %s"), *FilePath);
		return false;
	}

	return true;
}

bool GameOfLifeCore::SaveGameStateToFile(FString FilePath)
{
	UE_LOG(LogGolCore, Display, TEXT("Saving file: %s"), *FilePath);
	
	TArray<FString> FileLines;
	//Important to add the format specifier
	FileLines.Add("#Life 1.06");

	for (FVector2<int64> Cell : LiveCells)
	{
		FileLines.Add(FString::Printf(TEXT("%lld %lld"), Cell.X, Cell.Y));
	}
	
	return FFileHelper::SaveStringArrayToFile(FileLines, *FilePath);
}

void GameOfLifeCore::PrintGameStateToLog()
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

	for(int y = minY; y <= maxY; ++y)
	{
		FString Line = "";
		
		for(int x = minX; x <= maxX; ++x)
		{
			Line += LiveCells.Contains({x, y}) ? "X" : "O";
		}
		
		UE_LOG(LogGolCore, Display, TEXT("%s"), *Line);
	}
}

bool GameOfLifeCore::ParseStringToInt64(FString* string, int64* OutInt64)
{
	//this should actually check that it's a valid int64
	*OutInt64 = FCString::Atoi64(**string);
	return true;
}
