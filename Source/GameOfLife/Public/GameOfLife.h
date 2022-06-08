#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


/**
 * This is not really used at the moment, but the idea would be to expose GOLCore functionality to BP / UE at large
 */ 
class FGameOfLife : public IModuleInterface
{
public:

	static inline FGameOfLife& Get()
	{
		return FModuleManager::GetModuleChecked<FGameOfLife>("GameOfLifeCore");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GameOfLifeCore");
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
