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
	/*UPROPERTY()
	UCell* Adjacent_TL = nullptr; // top left adjacent
	UPROPERTY()
	UCell* Adjacent_TC = nullptr; // top center adjacent
	UPROPERTY()
	UCell* Adjacent_TR = nullptr; // top right adjacent 
	UPROPERTY()
	UCell* Adjacent_L = nullptr;  // left adjacent
	UPROPERTY()
	UCell* Adjacent_R = nullptr;  // right adjacent 
	UPROPERTY()
	UCell* Adjacent_BL = nullptr; // bottom left adjacent
	UPROPERTY()
	UCell* Adjacent_BC = nullptr; // bottom center adjacent
	UPROPERTY()
	UCell* Adjacent_BR = nullptr; // bottom right adjacent */

	void ShowAdjacentCells(int depth, FVector componentUpVector, UWorld* componentWorld);
	GridPosition GetAdjacentCellLocation(AdjecantDirections DirectionToGet) const;
	//UCell* GetAdjacentCell(AdjecantDirections DirectionToGet) const;
	void GetAdjacentGridPos(GridPosition& GridPosition, AdjecantDirections DirectionToGet);
	bool RaycastAdjacentObjects(int posX, int posY, FHitResult& result, FVector componentUpVector, UWorld* componentWorld,AdjecantDirections DirectionToGet);
	//void SetAdjacent(AdjecantDirections directionToSet, UCell* ObjectToSet);

private:
	const UCell* CellParent; 

};
