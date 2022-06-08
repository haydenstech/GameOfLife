#include "CoreMinimal.h"
#include "Misc/CommandLine.h"
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

	FString FileName = FString("Test.life");

	if(GameOfLife->LoadGameStateFromFile(FString("C:/dev/unreal-projects/game-of-life/Patterns/" + FileName)))
	{
		RequestEngineExit(TEXT("Failed to load Game Of Life from file"));
	}

	GameOfLife->PrintGameStateToLog();
	
	GameOfLife->SaveGameStateToFile(FString("C:/dev/unreal-projects/game-of-life/Patterns/" + FileName));
	
	GameOfLife.Release();
	
	RequestEngineExit(TEXT("DcPluginHeadless Main Exit"));
	FEngineLoop::AppPreExit();
	FEngineLoop::AppExit();

	return 0;
}


