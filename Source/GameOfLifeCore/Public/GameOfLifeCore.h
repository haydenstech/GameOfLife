// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VectorTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGolCore, Log, All);

/**
 * 
 */
class GAMEOFLIFECORE_API GameOfLifeCore
{
public:
	GameOfLifeCore();
	~GameOfLifeCore();

	bool LoadGameStateFromFile(FString FilePath);
	
	bool SaveGameStateToFile(FString FilePath);

	/**
	 * Checks for and removes cells in the current game state that would die in the next iteration.
	 * (Potentially useful for clearing up weird input data?)
	 */
	void SanitizeGameState();

	/**
	 * 
	 */
	void IterateSimulationState(uint8 Iterations = 1, bool bPrintIterationsToLog = false);

	/**
	 * DEBUG: prints a grid of the live cells in the current game state
	 */
	void PrintGameStateToLog();

	/**
	 * Returns the current set of live cells.
	 */
	const TSet<FVector2<int64>>* GetCurrentGameState() const { return &LiveCells; };
protected:
	//Might need more error checking for this in the future.
	bool ParseStringToInt64(FString* string, int64* OutInt64);
	
	TSet<FVector2<int64>> LiveCells;

	
};
