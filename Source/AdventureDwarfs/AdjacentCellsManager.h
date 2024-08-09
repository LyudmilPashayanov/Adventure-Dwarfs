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

	void ShowAdjacentCells(int depth);
	GridPosition GetAdjacentCellLocation(const  TPair<int, int> ColumnRowPair) const;
	UCell* GetAdjacentCell(TPair<int,int> colRowPair);
	bool RaycastAdjacentObjects(int posX, int posY, FHitResult& result);

private:
	int counter=0;
	const UCell* CellParent; 

};
