#include "GameOfLife/Public/GameOfLife.h"

void FGameOfLife::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FGameOfLife::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FGameOfLife, GameOfLife, "GameOfLife");