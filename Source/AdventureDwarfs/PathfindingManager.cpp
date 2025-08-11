// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingManager.h"
#include "Containers/BinaryHeap.h"
#include "Cell.h"
TArray<FIntVector> UPathfindingManager::FindPath(FIntVector StartCoordinates, FIntVector DestinationCoordinates)
{
    if (PathDirections.IsEmpty())
    {
        PopulateDirections();
    }

    // If pathfinding is already in progress, stop it
    if (bPathfindingInProgress)
    {
        GetWorld()->GetTimerManager().ClearTimer(PathfindingTimerHandle);
    }

    // Initialize pathfinding state
    bPathfindingInProgress = true;
    CurrentStartCoordinates = StartCoordinates;
    CurrentDestinationCoordinates = DestinationCoordinates;
    CurrentFinalPath.Empty();
    CurrentOpenNodesHeap.Clear();
    CurrentAllNodes.Empty();
    CurrentOpenNodes.Empty();
    CurrentVisited.Empty();

    // Set up starting node
    FPathNode startingPathNode = FPathNode(StartCoordinates, 0.0f, Heuristic(StartCoordinates, DestinationCoordinates), StartCoordinates);
    uint32 startingNodeIndex = CurrentAllNodes.Add(startingPathNode);
    CurrentOpenNodesHeap.Add(startingPathNode.F(), startingNodeIndex);
    CurrentOpenNodes.Add(StartCoordinates, startingPathNode);

    // Start the timer - 0.1f means 10 iterations per second
    GetWorld()->GetTimerManager().SetTimer(
        PathfindingTimerHandle, 
        this, 
        &UPathfindingManager::ProcessNextPathfindingStep, 
        0.1f, // Delay between iterations (adjust as needed)
        true  // Repeat
    );

    // Return empty path initially - will be filled as pathfinding progresses
    // You might want to use a delegate/callback to notify when pathfinding is complete
    return CurrentFinalPath;
}

void UPathfindingManager::ProcessNextPathfindingStep()
{
    // Check if we still have nodes to process
    if (CurrentOpenNodesHeap.Num() <= 0)
    {
        // No path found
        GetWorld()->GetTimerManager().ClearTimer(PathfindingTimerHandle);
        bPathfindingInProgress = false;
        UE_LOG(LogTemp, Warning, TEXT("No path found!"));
        // Optionally broadcast completion with empty path
        return;
    }

    // Process one iteration of pathfinding
    FPathNode Current;
    int BestFIndex = CurrentOpenNodesHeap.Top();
    Current = CurrentAllNodes[BestFIndex];

    CurrentOpenNodes.Remove(Current.Coordinates);
    CurrentOpenNodesHeap.Pop();
	
	bool debug = true;
	if (debug)
	{
		FString debugData = FString::Printf(TEXT("F=%.3f \n G=%.3f \n H=%.3f "), Current.F(), Current.G, Current.H);
		GridManager->GetCellAt(Current.Coordinates)->ShowPathfindingDebugData(debugData);
	}
	
    CurrentVisited.Add(Current.Coordinates, Current);

    // Check if we reached the destination
    if (Current.Coordinates == CurrentDestinationCoordinates)
    {
        // Stop the timer
        GetWorld()->GetTimerManager().ClearTimer(PathfindingTimerHandle);
        bPathfindingInProgress = false;

        // Reconstruct path
        FIntVector Step = CurrentDestinationCoordinates;
    	int counter = 0;
        while (Step != CurrentStartCoordinates)
        {
        	counter++;
        	GridManager->GetCellAt(Step)->Highlight(counter);		
            CurrentFinalPath.Insert(Step, 0);
            Step = CurrentVisited[Step].Parent;
        	
            UE_LOG(LogTemp, Warning, TEXT("Node %s: F=%.3f, G=%.3f, H=%.3f"), 
                *Step.ToString(), CurrentVisited[Step].F(), CurrentVisited[Step].G, CurrentVisited[Step].H);
        }
        CurrentFinalPath.Insert(CurrentStartCoordinates, 0);
        
        UE_LOG(LogTemp, Warning, TEXT("Path found! Length: %d"), CurrentFinalPath.Num());
        
        // Optionally broadcast completion
        // OnPathfindingComplete.Broadcast(CurrentFinalPath);
        return;
    }

    // Process neighbours
    for (FIntVector OffsetDirection : PathDirections)
    {
        FIntVector NeighbourCoord = Current.Coordinates + OffsetDirection;

        if (CurrentVisited.Contains(NeighbourCoord))
        {
            continue;
        }
        
        UCell* NeighbourCell = GridManager->GetCellAt(NeighbourCoord);
        if (NeighbourCell == nullptr || !NeighbourCell->IsWalkable())
        {
            continue;
        }
        
        float MovementCost = FVector(OffsetDirection).Size();
    	UE_LOG(LogTemp, Log, TEXT("direction being processed= %s MovementCost: %.3f"), *OffsetDirection.ToString(), MovementCost );
        float G = Current.G + MovementCost;
        float H = Heuristic(NeighbourCoord, CurrentDestinationCoordinates);
        FPathNode newNode = FPathNode(NeighbourCoord, G, H, Current.Coordinates);
        
        if (CurrentOpenNodes.Contains(NeighbourCoord) == false)
        {
            uint32 newNodeIndex = CurrentAllNodes.Add(newNode);
            CurrentOpenNodesHeap.Add(newNode.F(), newNodeIndex);
            CurrentOpenNodes.Add(NeighbourCoord, newNode);
        }
        else if (G < CurrentOpenNodes[NeighbourCoord].G)
        {
            uint32 newNodeIndex = CurrentAllNodes.Add(newNode);
            CurrentOpenNodesHeap.Add(newNode.F(), newNodeIndex);
            CurrentOpenNodes[NeighbourCoord] = newNode;
        }
    }
}

/*
TArray<FIntVector> UPathfindingManager::FindPath(FIntVector StartCoordinates, FIntVector DestinationCoordinates)
{
	if (PathDirections.IsEmpty())
	{
		PopulateDirections();
	}

	TArray<FIntVector> FinalPath;
	
	FBinaryHeap<float, uint32> OpenNodesHeap;
	TArray<FPathNode> AllNodes;
	TMap<FIntVector, FPathNode> OpenNodes;
	TMap<FIntVector, FPathNode> Visited;

	FPathNode startingPathNode = FPathNode(StartCoordinates, 0.0f, Heuristic(StartCoordinates, DestinationCoordinates) , StartCoordinates);

	uint32 startingNodeIndex = AllNodes.Add(startingPathNode);
	OpenNodesHeap.Add(startingPathNode.F(), startingNodeIndex);
	OpenNodes.Add(StartCoordinates, startingPathNode);
	
	while (OpenNodesHeap.Num() > 0)
	{
		FPathNode Current;
		int BestFIndex = OpenNodesHeap.Top();

		Current = AllNodes[BestFIndex];

		OpenNodes.Remove(Current.Coordinates);
		OpenNodesHeap.Pop();
		
		Visited.Add(Current.Coordinates, Current);

		if (Current.Coordinates == DestinationCoordinates)
		{
			// Reconstruct path
			FIntVector Step = DestinationCoordinates;
			while (Step != StartCoordinates)
			{
				FinalPath.Insert(Step, 0);
				Step = Visited[Step].Parent;
				UE_LOG(LogTemp, Warning, TEXT("Node %s: F=%.3f, G=%.3f, H=%.3f"), *Step.ToString(), Visited[Step].F(), Visited[Step].G, Visited[Step].H);
			}
			FinalPath.Insert(StartCoordinates, 0);
			return FinalPath;
		}

		// Review if all 26 neighbouring directions are potential nodes to the destination.
		for (FIntVector OffsetDirection : PathDirections)
		{
			FIntVector NeighbourCoord = Current.Coordinates + OffsetDirection;

			if (Visited.Contains(NeighbourCoord))
			{
				continue;
			}
			UCell* NeighbourCell = GridManager->GetCellAt(NeighbourCoord);
			if (NeighbourCell == nullptr || !NeighbourCell->IsWalkable())
			{
				continue;
			}
			float MovementCost = OffsetDirection.Size(); // This makes diagonal moves cost more than straight moves.
			float G = Current.G + MovementCost;
			float H = Heuristic(NeighbourCoord, DestinationCoordinates);
			FPathNode newNode = FPathNode(NeighbourCoord, G, H, Current.Coordinates);

			bool debug = true;
			if (debug)
			{
				FString debugData = FString::Printf(TEXT("F=%.3f \n G=%.3f \n H=%.3f "), newNode.F(), newNode.G, newNode.H);
				NeighbourCell->ShowPathfindingDebugData(debugData);
			}
			if (OpenNodes.Contains(NeighbourCoord) == false) // If this node doesn't exist in potential checks. Add it.
			{
				uint32 newNodeIndex = AllNodes.Add(newNode);
				OpenNodesHeap.Add(newNode.F(), newNodeIndex);
				OpenNodes.Add(NeighbourCoord, newNode);
			}
			else if (G < OpenNodes[NeighbourCoord].G) // If this node already exists, but this is a better option. Replace the existing node.
			{
				uint32 newNodeIndex = AllNodes.Add(newNode);
				OpenNodesHeap.Add(newNode.F() ,newNodeIndex);
				OpenNodes[NeighbourCoord] = newNode;
			}
		}
	}

	return FinalPath;
}
*/

void UPathfindingManager::PopulateDirections() // TODO: Add directions which check -2,-3,-4 nodes below the player. Falling should be acceptable at some distance :).
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
	// Manhattan
	//return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y) + FMath::Abs(A.Z - B.Z);
	
	// Chebyshev
	//int dx = FMath::Abs(A.X - B.X);
	//int dy = FMath::Abs(A.Y - B.Y);
	//int dz = FMath::Abs(A.Z - B.Z);
	//
	//return FMath::Max3(dx, dy, dz);


	// Eucledian
	float dx = (float)(A.X - B.X);
	float dy = (float)(A.Y - B.Y);
	float dz = (float)(A.Z - B.Z);
    
	return FMath::Sqrt(dx*dx + dy*dy + dz*dz);
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
