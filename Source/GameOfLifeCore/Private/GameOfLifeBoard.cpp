#include "GameOfLifeBoard.h"

DEFINE_LOG_CATEGORY(LogGameOfLifeBoard);

GameOfLifeBoard::GameOfLifeGridSection::GameOfLifeGridSection(FVector2<int64> Id, uint16 GridSizeVal): GridSize(GridSizeVal), GridId(Id)
{
	GridWorldSpaceOrigin = GridId * GridSize;
	CellStates = MakeUnique<TArray<bool>>();
	CellStates->Init(false, GridSize * GridSize);
}

TSet<FVector2<int64>> GameOfLifeBoard::GameOfLifeGridSection::GetLiveCells()
{
	TSet<FVector2<int64>> OurLiveCells;

	for(int i = 0; i < CellStates->Num(); i++)
	{
		if((*CellStates)[i])
		{
			OurLiveCells.Add({(i % GridSize) + GridWorldSpaceOrigin.X, i / GridSize + GridWorldSpaceOrigin.Y});
		}
	}

	return OurLiveCells;
}

void GameOfLifeBoard::GameOfLifeGridSection::UpdateSimulationState(const TUniquePtr<GameOfLifeBoard> *LastState)
{
	FVector2<int64> CurrentCell;
	int CellNeighbors;
	
	for(int i = 0; i < CellStates->Num(); i++)
	{
		//Convert out local 1D array into global corrodinates
		CurrentCell.X = (i % GridSize) + GridWorldSpaceOrigin.X;
		CurrentCell.Y = i / GridSize + GridWorldSpaceOrigin.Y;

		CellNeighbors = GetLiveNeighborCellsCount(CurrentCell, LastState);
		
		if(LastState->Get()->IsCellAlive(CurrentCell))
		{
			//Alive cells with less than 2 or more than 3 neighbors die.
			(*CellStates)[i] = !(CellNeighbors < 2 || CellNeighbors > 3);
		}
		else
		{
			//Dead cells with exactly 3 neighbors become alive.
			if(CellNeighbors == 3)
			{
				(*CellStates)[i] = true;
			}
			else
			{
				(*CellStates)[i] = false;
			}
			//(*CellStates)[i] = CellNeighbors == 3;
		}
	}
}

TUniquePtr<GameOfLifeBoard::GameOfLifeGridSectionState> GameOfLifeBoard::GameOfLifeGridSection::GetGolGridSquareState()
{
	//I serve the soviet union
	auto OurState = MakeUnique<GameOfLifeBoard::GameOfLifeGridSectionState>();
	int X, Y;
	
	for(int i = 0; i < CellStates->Num(); i++)
	{
		if((*CellStates)[i])
		{
			OurState->bIsEmpty = false;
			X = (i % GridSize);
			Y = (i / GridSize);

			//Check diagonals: Top left / Bottom Left / Top Right / Bottom Right
			
			if(X == 0 && Y == 0)
			{
				OurState->RequiredNeighbors.Add( GridId + TopLeftOffset );
			}
			else if(X == 0 && Y == GridSize - 1)
			{
				OurState->RequiredNeighbors.Add( GridId + BottomLeftOffset );
			}
			else if(X == GridSize - 1 && Y == 0)
			{
				OurState->RequiredNeighbors.Add( GridId + TopRightOffset );
			}
			else if(X == GridSize - 1 && Y == GridSize - 1)
			{
				OurState->RequiredNeighbors.Add( GridId + BottomRightOffset );
			}

			//Check top / right / bottom / left
			if(X == 0)
			{
				OurState->RequiredNeighbors.Add( GridId + LeftOffset );
			}
			else if(X == GridSize - 1)
			{
				OurState->RequiredNeighbors.Add( GridId + RightOffset );
			}

			if(Y == 0)
			{
				OurState->RequiredNeighbors.Add( GridId + TopOffset );
			}
			else if(Y == GridSize - 1)
			{
				OurState->RequiredNeighbors.Add( GridId + BottomOffset );
			}
		}
	}

	return OurState;
}

bool GameOfLifeBoard::GameOfLifeGridSection::IsCellAlive(FVector2<int64> Cell)
{
	//TODO: This needs to take into account if the resultant cell is outside of INT64_MIN <-> INT64_MAX

	int64 LocalXPos = Cell.X - GridWorldSpaceOrigin.X;
	int64 LocalYPos = Cell.Y - GridWorldSpaceOrigin.Y;
	int64 ArrayPosition = LocalXPos + (LocalYPos * GridSize);

	if(ArrayPosition >= CellStates->Num())
	{
		UE_LOG(LogGameOfLifeBoard, Error, TEXT("Attempting to access out of range position!"));
		return false;
	}

	return (*CellStates.Get())[ArrayPosition];
}

void GameOfLifeBoard::GameOfLifeGridSection::SetCellIsAlive(FVector2<int64> Cell, bool bNewCellState)
{
	int64 ArrayPosition = Cell.X - GridWorldSpaceOrigin.X + ((Cell.Y - GridWorldSpaceOrigin.Y) * GridSize);

	if(ArrayPosition >= CellStates->Num())
	{
		UE_LOG(LogGameOfLifeBoard, Error, TEXT("Attempting to access out of range position!"));
		return;
	}
	
	(*CellStates.Get())[ArrayPosition] = bNewCellState;
}

int GameOfLifeBoard::GameOfLifeGridSection::GetLiveNeighborCellsCount(FVector2<int64> Cell, const TUniquePtr<GameOfLifeBoard>* State)
{
	int NeighborCount = 0;

	for(FVector2<int64> NeighborOffset : NeighborCellOffsetArray)
	{
		NeighborCount += State->Get()->IsCellAlive(Cell + NeighborOffset);
	}

	return NeighborCount;
}

GameOfLifeBoard::GameOfLifeBoard::GameOfLifeBoard(uint16 GridSize): GridSize(GridSize)
{
	GridIdToGridSquareMap = MakeUnique<TMap<FVector2<int64>, GameOfLifeGridSection*>>();
}

GameOfLifeBoard::GameOfLifeBoard::~GameOfLifeBoard()
{
	GridIdToGridSquareMap.Reset();
}

TSet<FVector2<int64>> GameOfLifeBoard::GameOfLifeBoard::GetLiveCells()
{
	TSet<FVector2<int64>> TotalLiveCells;

	for(auto Origin : GetAllGridIds())
	{
		TotalLiveCells.Append(GridIdToGridSquareMap->FindRef(Origin)->GetLiveCells());
	}

	return TotalLiveCells;
}

void GameOfLifeBoard::GameOfLifeBoard::ClearState()
{	
	GridIdToGridSquareMap->Reset();
}

void GameOfLifeBoard::GameOfLifeBoard::LoadStateFromLiveCellSet(TSet<FVector2<int64>>* LiveCells)
{
	for(auto Cell : *LiveCells)
	{
		SetCellIsAlive(Cell, true);
	}
}

void GameOfLifeBoard::GameOfLifeBoard::UpdateSimulationFromLastState(const TUniquePtr<GameOfLifeBoard> *LastState)
{
	TArray<FVector2<int64>> LastStateGridIds = LastState->Get()->GetAllGridIds();

	//Parse all of the previous state's required grid squares, and add them to this state (so they will be checked for this iteration)
	TSet<FVector2<int64>> ActiveGridIds;

	//Parse the grid states form the last simulation step to figure out which grids are needed for the next simulation state.
	for (FVector2<int64> CurrentGridId : LastStateGridIds)
	{
		auto CurrentGridState = LastState->Get()->GetGridSquareStateById(CurrentGridId);
		if(!CurrentGridState->bIsEmpty)
		{
			ActiveGridIds.Add(CurrentGridId);
			for(auto NeighborGridId : CurrentGridState.Get()->RequiredNeighbors)
			{
				ActiveGridIds.Add(NeighborGridId);
			}
		}		
		CurrentGridState.Reset();
	}

	//Remove all states that aren't required from our current state
	for (FVector2<int64> CurrentGridId : GetAllGridIds())
	{
		if(!ActiveGridIds.Contains(CurrentGridId))
		{
			GridIdToGridSquareMap->Remove(CurrentGridId);
			ActiveGridIds.Remove(CurrentGridId);
		}
	}

	//If there are new required grids, add them before we start our simulation.
	for(FVector2<int64> NewGridId : ActiveGridIds)
	{
		if(!GridIdToGridSquareMap->Contains(NewGridId))
		{
			GridIdToGridSquareMap->Add( NewGridId, new GameOfLifeGridSection(NewGridId, GridSize) );
		}
	}

	//Finally, get the updated list of grid origins, and update all the grids for this simulation step
	TArray<FVector2<int64>> GridIdsToUpdate = GetAllGridIds();

	//This would be fairly easy to multi-thread, last state is constant and no grid square changes data outside of itself.
	for (FVector2<int64> GridId : GridIdsToUpdate)
	{
		GridIdToGridSquareMap->FindRef(GridId)->UpdateSimulationState(LastState);
	}	
}

TUniquePtr<GameOfLifeBoard::GameOfLifeGridSectionState> GameOfLifeBoard::GameOfLifeBoard::GetGridSquareStateById(FVector2<int64> GridOrigin) const
{
	if(GridIdToGridSquareMap->Contains(GridOrigin))
	{
		return GridIdToGridSquareMap->FindRef(GridOrigin)->GetGolGridSquareState();
	}
	else
	{
		//Grid doesn't exist, therefore it's entirely empty.
		return MakeUnique<GameOfLifeBoard::GameOfLifeGridSectionState>();
	}
}

const TArray<FVector2<int64>> GameOfLifeBoard::GameOfLifeBoard::GetAllGridIds()
{
	TArray<FVector2<int64>> GridIds;
	GridIdToGridSquareMap->GetKeys(GridIds);
	return GridIds;
}

bool GameOfLifeBoard::GameOfLifeBoard::IsCellAlive(FVector2<int64> Cell) const
{
	FVector2<int64> GridSquareOrigin = GetCellParentGridId(Cell);

	if(GridIdToGridSquareMap->Contains(GridSquareOrigin))
	{
		return GridIdToGridSquareMap->FindRef(GridSquareOrigin)->IsCellAlive(Cell);
	}
	//If the gridSquare doesn't exist then we can assume that the cell doesn't exist.
	return false;
}

void GameOfLifeBoard::GameOfLifeBoard::SetCellIsAlive(FVector2<int64> Cell, bool bIsActive, bool bAllowGridSquareCreation)
{
	auto GridSquareId = GetCellParentGridId(Cell);

	if(!GridIdToGridSquareMap->Contains(GridSquareId))
	{
		GridIdToGridSquareMap->Add( GridSquareId, new GameOfLifeGridSection(GridSquareId, GridSize) );
	}

	GridIdToGridSquareMap->FindRef(GridSquareId)->SetCellIsAlive(Cell, bIsActive);
}

FVector2<int64> GameOfLifeBoard::GameOfLifeBoard::GetCellParentGridId(FVector2<int64> Cell) const
{
	/*
	 * This feels incredibly silly, but I can't figure out a mathematical way (with int math) to get the negative bounding correct.
	 */
	int64 X = 0;
	int64 Y = 0;

	if(Cell.X > 0)
	{
		X = Cell.X / GridSize;
	}
	else
	{
		//Negative cells need to essentially use the floor of X / gridsize, but due to interger math we lose the trailing information...
		X = -(FMath::Abs(Cell.X) / GridSize);
		if(Cell.X % GridSize != 0) X -= 1;
	}

	if(Cell.Y > 0)
	{
		Y = Cell.Y / GridSize;
	}
	else
	{
		//Negative cells need to essentially use the floor of Y / gridsize, but due to interger math we lose the trailing information...
		Y = -(FMath::Abs(Cell.Y) / GridSize);
		if(Cell.Y % GridSize != 0) Y -= 1;
	}
	
	return FVector2<int64>( X, Y );
}
