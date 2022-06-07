#include "GameOfLife/Public/GameOfLife.h"

void FGameOfLife::StartupModule()
{
	IModuleInterface::StartupModule();
	UE_LOG(LogTemp, Display, TEXT("Hello from GOL Module startup"));
}

void FGameOfLife::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FGameOfLife, GameOfLife, "GameOfLife");