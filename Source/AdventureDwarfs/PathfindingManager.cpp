// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingManager.h"

#include "Cell.h"

TArray<FIntVector> UPathfindingManager::FindPath(FIntVector StartCoordinates, FIntVector DestinationCoordinates)
{
	if (PathDirections.IsEmpty())
	{
		PopulateDirections();
	}
	
	TArray<FIntVector> FinalPath;
	TMap<FIntVector, PathNode> OpenMap;
	TMap<FIntVector, PathNode> Visited;
	
	OpenMap.Add(StartCoordinates, PathNode(StartCoordinates, 0.0f, Heuristic(StartCoordinates, DestinationCoordinates) , StartCoordinates));

	while (OpenMap.Num() > 0)
	{
		PathNode Current;
		float BestF = FLT_MAX;

		for (const TPair<FIntVector, PathNode>& Pair : OpenMap) // TODO: Find the lowest F in a map with a binary tree container for optimizations 
		{
			const PathNode& Node = Pair.Value;
			if (Node.F() < BestF)
			{
				BestF = Node.F();
				Current = Node;
			}
		}

		OpenMap.Remove(Current.Coordinates);
		Visited.Add(Current.Coordinates, Current);

		if (Current.Coordinates == DestinationCoordinates)
		{
			// Reconstruct path
			FIntVector Step = DestinationCoordinates;
			while (Step != StartCoordinates)
			{
				FinalPath.Insert(Step, 0);
				Step = Visited[Step].Parent;
			}
			FinalPath.Insert(StartCoordinates, 0);
			return FinalPath;
		}

		// 26 directions
		for (FIntVector direction : PathDirections)
		{
			FIntVector NeighborCoord = Current.Coordinates + FIntVector(direction.X, direction.Y, direction.Z);

			if (Visited.Contains(NeighborCoord))
			{
				continue;
			}
			UCell* NeighborCell = GridManager->GetCellAt(NeighborCoord);
			if (NeighborCell == nullptr || !NeighborCell->IsWalkable())
			{
				continue;
			}
			float MovementCost = FVector(direction.X, direction.Y, direction.Z).Size(); // Cost for diagonals
			float G = Current.G + MovementCost; ;
			float H = Heuristic(NeighborCoord, DestinationCoordinates);

			if (OpenMap.Contains(NeighborCoord) == false) // If a path doesn't exist to be checked. Add it.
			{
				OpenMap.Add(NeighborCoord, PathNode(NeighborCoord, G, H, Current.Coordinates));
			}
			else if (G < OpenMap[NeighborCoord].G) // If a path exists, but this is a better option. Replace the existing path.
			{
				OpenMap.Remove(NeighborCoord);
				OpenMap.FindOrAdd(NeighborCoord,PathNode(NeighborCoord, G, H, Current.Coordinates));
			}
		}
	}

	return FinalPath;
}

void UPathfindingManager::SetGridManager(AGridManager* const InGridManager)
{
	if (GridManager  == nullptr)
	{
		GridManager = InGridManager;
		if (GridManager)
		{
			UE_LOG(LogTemp, Log, TEXT("GridManager successfully set in PathfindingManager"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GridManager was NULL when trying to set it in PathfindingManager!"));
		}
	}
}

void UPathfindingManager::HighlightPath(FIntVector Coordinates, FIntVector IntVector)
{
	TArray<FIntVector> path = FindPath(Coordinates, IntVector);
	int counter = 0;
	for (auto cellCoordinates: path)
	{
		counter++;
		UE_LOG(LogTemp, Log, TEXT("counter of path: %d "), counter);
		UE_LOG(LogTemp, Log, TEXT("path X: %d , Y: %d , Z: %d "), cellCoordinates.X, cellCoordinates.Y, cellCoordinates.Z);
		GridManager->GetCellAt(cellCoordinates)->Highlight(counter);		
	}
}

void UPathfindingManager::PopulateDirections()
{
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			for (int z = -1; z <= 1; ++z)
			{
				if (x == 0 && y == 0 && z == 0)
					continue;
				
				PathDirections.Add(FIntVector(x, y, z));
			}
		}
	}
}

float UPathfindingManager::Heuristic(const FIntVector& A, const FIntVector& B)
{
	return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y) + FMath::Abs(A.Z - B.Z); // Manhattan
}
