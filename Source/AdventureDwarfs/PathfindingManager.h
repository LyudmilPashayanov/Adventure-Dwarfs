// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridManager.h"
#include "Containers/BinaryHeap.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathfindingManager.generated.h"

USTRUCT(BlueprintType)
struct ADVENTUREDWARFS_API FPathNode
{
	GENERATED_BODY()

	FIntVector Coordinates;
	FIntVector Parent;
	float G;
	float H;

	FPathNode() : Coordinates(FIntVector{0,0,0}), Parent(FIntVector{0,0,0}), G(0), H(0)
	{}

	FPathNode(const FIntVector& InCoord, float InG, float InH, const FIntVector& InParent)
		: Coordinates(InCoord), Parent(InParent), G(InG), H(InH) {}

	float F() const 
	{
		float F = G + H;
    
		// Add tiny tie-breaker to prefer straighter paths
		float TieBreaker = H * 0.001f;
    
		return F - TieBreaker;
	}
	bool operator==(const FPathNode& Other) const { return Coordinates == Other.Coordinates; }
	bool operator<(const FPathNode& Other) const { return F() < Other.F(); }
};

UCLASS()
class ADVENTUREDWARFS_API UPathfindingManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	TArray<FIntVector> FindPath(FIntVector StartCoordinates, FIntVector DestinationCoordinates);
	void SetGridManager(AGridManager* const InGridManager);
	void HighlightPath(FIntVector Coordinates, FIntVector IntVector);

private:

	UPROPERTY()
	AGridManager* GridManager;

	TArray<FIntVector> PathDirections;
	void PopulateDirections();
	float Heuristic(const FIntVector& A, const FIntVector& B);

	////////////////////////////////////////////////////////
	UPROPERTY()
	FTimerHandle PathfindingTimerHandle;

	// Member variables to store pathfinding state between timer calls
	UPROPERTY()
	TArray<FIntVector> CurrentFinalPath;

	FBinaryHeap<float, uint32> CurrentOpenNodesHeap;

	UPROPERTY()
	TArray<FPathNode> CurrentAllNodes;

	UPROPERTY()
	TMap<FIntVector, FPathNode> CurrentOpenNodes;

	UPROPERTY()
	TMap<FIntVector, FPathNode> CurrentVisited;

	UPROPERTY()
	FIntVector CurrentDestinationCoordinates;

	UPROPERTY()
	FIntVector CurrentStartCoordinates;

	UPROPERTY()
	bool bPathfindingInProgress;

	// Declare the timer function
	UFUNCTION()
	void ProcessNextPathfindingStep();

	// Optional: Delegate to notify when pathfinding is complete
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPathfindingComplete, const TArray<FIntVector>&, FinalPath);
};

