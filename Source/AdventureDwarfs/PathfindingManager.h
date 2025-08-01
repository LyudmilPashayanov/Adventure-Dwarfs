// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathfindingManager.generated.h"

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
};


struct PathNode
{
	FIntVector Coordinates;
	FIntVector Parent;
	float G;
	float H;

	PathNode() : Coordinates(FIntVector{0,0,0}), G(0), H(0), Parent(FIntVector{0,0,0})
	{}

	PathNode(const FIntVector& InCoord, float InG, float InH, const FIntVector& InParent)
		: Coordinates(InCoord), Parent(InParent), G(InG), H(InH) {}

	float F() const { return G + H; }

	bool operator==(const PathNode& Other) const { return Coordinates == Other.Coordinates; }
};
