#pragma once
#include "VectorTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameOfLifeBoard, Log, All);

/**
 * Representation of the game board as a whole, handles allocating grids / deallocating as required.
 */
class GameOfLifeBoard
{
public:
	GameOfLifeBoard(uint16 GridSize);
	~GameOfLifeBoard();

	TSet<FVector2<int64>> GetLiveCells();

	void ClearState();

	//Sets up the game state from a set of active cells
	void LoadStateFromLiveCellSet(TSet<FVector2<int64>>* LiveCells);

	//Calculates the new board state based upon the previous iteration's state.
	void UpdateSimulationFromLastState(const TUniquePtr<GameOfLifeBoard>* LastState);

protected:

	struct GameOfLifeGridSectionState
	{
		bool bIsEmpty = true;

		TSet<FVector2<int64>> RequiredNeighbors;
	};

	//If a requested cell state is outside of INT64_MIN <-> INT64_MAX it is assumed dead, and can't be written to.
	//GolGridSquare represents a manageable chunk of the GOL board, encapsulated to be easier to work with mentally and also for multithreading
	class GameOfLifeGridSection
	{
	public:
		GameOfLifeGridSection(FVector2<int64> Id, uint16 GridSizeVal);

		TSet<FVector2<int64>> GetLiveCells();

		void UpdateSimulationState(const TUniquePtr<GameOfLifeBoard>* LastState);

		//This could be refactored to be contained within the board at all times..
		TUniquePtr<GameOfLifeGridSectionState> GetGolGridSquareState();

		bool IsCellAlive(FVector2<int64> Cell);

		void SetCellIsAlive(FVector2<int64> Cell, bool bNewCellState);

	protected:
		int GetLiveNeighborCellsCount(FVector2<int64> Cell, const TUniquePtr<GameOfLifeBoard>* State);

		//X / Y size of each grid in the game board.
		uint16 GridSize;

		//ID of this grid square in the game board as a whole.
		FVector2<int64> GridId;

		//Used to convert local space to game space.
		FVector2<int64> GridWorldSpaceOrigin;

		//This could absolutely be a case for packed booleans, Will investigate later.
		/*
		 * 1D array of the cells in this grid square, 1D for speed. in LOCAL SPACE (Origin is 0,0)
		 */
		TUniquePtr<TArray<bool>> CellStates;

		/** Simulation space visualisation:
		 *
		 *		    -Y
		 *			|
		 *			|
		 *  -X -----+----- +X
		 *			|
		 *			|
		 *			+Y
		 *
		 *	Statics defined below are mostly for legibility of code.
		 */

		static constexpr FVector2<int64> TopLeftOffset{-1, -1};
		static constexpr FVector2<int64> TopOffset{0, -1};
		static constexpr FVector2<int64> TopRightOffset{1, -1};

		static constexpr FVector2<int64> LeftOffset{-1, 0};
		static constexpr FVector2<int64> RightOffset{1, 0};

		static constexpr FVector2<int64> BottomLeftOffset{-1, 1};
		static constexpr FVector2<int64> BottomOffset{0, 1};
		static constexpr FVector2<int64> BottomRightOffset{1, 1};

		const TArray<FVector2<int64>> NeighborCellOffsetArray{
			TopLeftOffset, TopOffset, TopRightOffset, LeftOffset, RightOffset, BottomLeftOffset, BottomOffset,
			BottomRightOffset
		};
	};

	//Returns all currently allocated grids in the GridSquareMap
	const TArray<FVector2<int64>> GetAllGridIds();
	
	bool IsCellAlive(FVector2<int64> Cell) const;
	
	void SetCellIsAlive(FVector2<int64> Cell, bool bIsActive, bool bAllowGridSquareCreation = true);
	
	TUniquePtr<GameOfLifeGridSectionState> GetGridSquareStateById(FVector2<int64> GridOrigin) const;
	
	FVector2<int64> GetCellParentGridId(FVector2<int64> Cell) const;

	uint16 GridSize;

	TUniquePtr<TMap<FVector2<int64>, GameOfLifeGridSection*>> GridIdToGridSquareMap;
};
