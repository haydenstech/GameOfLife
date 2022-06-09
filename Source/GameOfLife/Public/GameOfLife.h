#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


/**
 * This is not really used at the moment, but the idea would be to expose GOLCore functionality to BP's etc for a visual interface.
 * (In order to be used with the headless target the GOLCore module can't touch engine / rendering stuff)
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
