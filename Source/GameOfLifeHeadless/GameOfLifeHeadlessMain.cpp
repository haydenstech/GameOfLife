#include "CoreMinimal.h"
#include "Runtime/Launch/Public/RequiredProgramMainCPPInclude.h"

#include "stdio.h"
#include "Launch/Public/LaunchEngineLoop.h"
#include "GameOfLifeCore/Public/GameOfLifeCore.h"

//GameOfLifeHeadless is shown as an error, but it is indeed fine.
IMPLEMENT_APPLICATION(GameOfLifeHeadless, "GameOfLifeHeadless");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	if (GEngineLoop.PreInit(ArgC, ArgV) != 0) // NOLINT
	{
		checkf(false, TEXT("Engine Preinit Failed"));
		return -1;
	}

	TUniquePtr<GameOfLifeCore> GameOfLife = MakeUnique<GameOfLifeCore>();

	int Iterations = 10;
	
	FString InputFilePath = FString("../../Patterns/Glider.life");
	FString OutputFilePath = FString("../../Patterns/Result.life");

	//Fetch command line arg options
	FString CmdInputFilePath;
	if (FParse::Value(FCommandLine::Get(), TEXT("InputFilepath"), CmdInputFilePath)) 
	{
		InputFilePath = CmdInputFilePath;
	}
	
	FString CmdOutputFilePath;
	if (FParse::Value(FCommandLine::Get(), TEXT("OutputFilepath"), CmdOutputFilePath)) 
	{
		OutputFilePath = CmdOutputFilePath;
	}

	FString IterationsString;
	if (FParse::Value(FCommandLine::Get(), TEXT("Iterations"), IterationsString)) 
	{
		Iterations = FCString::Atoi(*IterationsString);
	}

	if(!GameOfLife->LoadGameStateFromFile(InputFilePath))
	{
		RequestEngineExit(TEXT("Failed to load Game Of Life state from file"));
		FEngineLoop::AppPreExit();
		FEngineLoop::AppExit();

		return 0;
	}

	GameOfLife->IterateSimulationState(Iterations);
	
	GameOfLife->SaveGameStateToFile(OutputFilePath);
	
	GameOfLife.Reset();
	
	RequestEngineExit(TEXT("GameOfLifeHeadless Main Exit"));
	FEngineLoop::AppPreExit();
	FEngineLoop::AppExit();

	return 0;
}


