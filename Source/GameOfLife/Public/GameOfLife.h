#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

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
