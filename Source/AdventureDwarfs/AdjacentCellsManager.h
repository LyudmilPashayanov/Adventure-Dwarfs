// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AdjecantDirections.h"
#include "GridPosition.h"
#include "CoreMinimal.h"
#include "Cell.h"

enum class AdjecantDirections;
class GridPosition;

class ADVENTUREDWARFS_API AdjacentCellsManager
{
public:
	AdjacentCellsManager(const UCell* ParentCell);

	void ShowAdjacentCells(int depth, FVector componentUpVector, UWorld* componentWorld, UCell* initiatorCell);
	GridPosition GetAdjacentCellLocation(AdjecantDirections DirectionToGet) const;
	void GetAdjacentGridPos(GridPosition& GridPosition, AdjecantDirections DirectionToGet);
	bool RaycastAdjacentObjects(int posX, int posY, FHitResult& result, FVector componentUpVector, UWorld* componentWorld,AdjecantDirections DirectionToGet);

private:
	UCell* lastInitiatorCell;
	const UCell* CellParent; 

};
