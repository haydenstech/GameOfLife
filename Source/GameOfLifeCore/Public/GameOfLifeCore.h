// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameOfLifeBoard.h"
#include "VectorTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameOfLifeCore, Log, All);

/**
 * Core set of functionality for simulating Conway's Game of life.
 * Can't use UE4 Engine code so as to be able to run on a headless target.
 */
class GAMEOFLIFECORE_API GameOfLifeCore
{
public:
	/**
	 *	GridSizeVal determines the grid size to split the Game of Life board into,
	 *	Low values (~10 - ~50) could potentially reduce execution time with small, centralized cell counts
	 */
	GameOfLifeCore(uint16 GridSizeVal = 128);

	~GameOfLifeCore();

	/**
	 * Loads a .Life file from the given file path, returns false if the game state was unable to be loaded for any reason (Specific reasons are logged.)
	 * This should probably be a return code instead of a bool.
	 */
	bool LoadGameStateFromFile(FString FilePath);

	/**
	 * Saves a .Life file to the given filepath from the current game state. Returns false if the file fails to save.
	 */
	bool SaveGameStateToFile(FString FilePath);

	/**
	 * Iterates the game board by the given iteration count and then saves the result in the current game state.
	 * If print iterations to log is true then a crude representation of the game state will printed to log each iteration (Very slow!)
	 */
	void IterateSimulationState(uint16 Iterations = 1, bool bPrintIterationsToLog = false);

	/**
	 * DEBUG: prints a grid of the live cells in the current game state to the log. (not fast or pretty)
	 */
	void DebugPrintGameStateToLog();

	/**
	 * Returns a TSet of the live cells in the current game state.
	 */
	const TSet<FVector2<int64>>* GetCurrentGameState() const { return &LiveCells; };
	
protected:
	
	//Very careful String to int64 parser, returns false if the input string is outside of the INT64 range, or is in any way unsuitable.
	bool ParseStringToInt64(FString* String, int64* OutInt64);

	//Current live cells in the simulation, used to hold state between iteration cycles.
	TSet<FVector2<int64>> LiveCells;

	//Grid size (X/Y) of each grid that makes up the Game of Life 'board'. Grid size is a balance between being able to effectively parallelize, and not splitting up the board to a ridiculous degree
	uint16 GridSize;

	TUniquePtr<GameOfLifeBoard> CurrentState;
	TUniquePtr<GameOfLifeBoard> NextState;
	
};
