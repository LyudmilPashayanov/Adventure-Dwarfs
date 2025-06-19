// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FGridPosition.h"
#include "CoreMinimal.h"
#include "Cell.h"

enum class AdjecantDirections;
struct FGridPosition;

class ADVENTUREDWARFS_API AdjacentCellsManager
{
public:
	AdjacentCellsManager(const UCell* ParentCell);

	void ShowAdjacentCells(int depth);
	FGridPosition GetAdjacentCellLocation(const  TPair<int, int> ColumnRowPair) const;
	TArray<UCell*> GetAdjacentCell(TPair<int,int> colRowPair);
	bool RaycastAdjacentObjects(int posX, int posY, FHitResult& result);
	TArray<TPair<int, int>> combinations;

private:
	int counter=0;
	const UCell* CellParent; 

};
