#include "GameOfLife/Public/GameOfLife.h"
#include "GameOfLifeCore.h"

void FGameOfLife::StartupModule()
{
	IModuleInterface::StartupModule();

	TUniquePtr<GameOfLifeCore> GameOfLife = MakeUnique<GameOfLifeCore>();
	
	FString FileName = FString("Test.life");

	if(!GameOfLife->LoadGameStateFromFile(FString("C:/dev/unreal-projects/game-of-life/Patterns/" + FileName)))
	{
		UE_LOG(LogTemp, Display, TEXT("Failed to load game from file"));
	}
	else
	{	
		GameOfLife->IterateSimulationState(1000);
	
		GameOfLife->SaveGameStateToFile(FString("C:/dev/unreal-projects/game-of-life/Patterns/" + FileName));
	}
	
	GameOfLife.Reset();
}

void FGameOfLife::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FGameOfLife, GameOfLife, "GameOfLife");