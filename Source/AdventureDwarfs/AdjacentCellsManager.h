// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GridPosition.h"
#include "CoreMinimal.h"
#include "Cell.h"

enum class AdjecantDirections;
class GridPosition;

class ADVENTUREDWARFS_API AdjacentCellsManager
{
public:
	AdjacentCellsManager(const UCell* ParentCell);

	void ShowAdjacentCells(int depth, FVector componentUpVector, UWorld* componentWorld);
	GridPosition GetAdjacentCellLocation(const  TPair<int, int> RowColumnPair) const;
	bool RaycastAdjacentObjects(int posX, int posY, FHitResult& result, FVector componentUpVector, UWorld* componentWorld);

private:
	int counter=0;
	UCell* lastInitiatorCell;
	const UCell* CellParent; 

};
