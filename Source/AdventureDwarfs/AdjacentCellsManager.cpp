// Fill out your copyright notice in the Description page of Project Settings.

#include "AdjacentCellsManager.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"

AdjacentCellsManager::AdjacentCellsManager(const UCell* ParentCell)
{
	CellParent = ParentCell;
}

void AdjacentCellsManager::ShowAdjacentCells(int depth, FVector componentUpVector, UWorld* componentWorld, UCell* initiatorCell)
{
	if(lastInitiatorCell != nullptr && lastInitiatorCell == initiatorCell)
		return;

	
	//counter++;
	//UE_LOG(LogTemp, Log, TEXT("ShowAdjacentCells counter: %d , Cell Row = %d, Cell column= %d, Chunk parent name = %s"),counter, CellParent->Row,CellParent->Column,*CellParent->ChunkParent->GetName());

	depth--;
	FHitResult Hit;
	lastInitiatorCell = initiatorCell;
	for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
	{
		const AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
		GridPosition positionToCheck = GetAdjacentCellLocation(currentEnumValue);
		if (RaycastAdjacentObjects(positionToCheck.X, positionToCheck.Y, Hit, componentUpVector, componentWorld,currentEnumValue))
		{
			GetAdjacentGridPos(positionToCheck, currentEnumValue);
			UCell* cell = Cast<AChunk>(Hit.GetActor())->GetCell((positionToCheck));
			cell->ShowCell();
			if (depth > 0)
			{
				cell->ShowAdjacentCells(depth, initiatorCell);
			}
		}
	}
}

GridPosition AdjacentCellsManager::GetAdjacentCellLocation(AdjecantDirections DirectionToGet) const
{
	const int ParentLocationX = CellParent->GetComponentLocation().X;
	const int ParentLocationY = CellParent->GetComponentLocation().Y;
	const int halfSize = CellParent->CellMesh->GetStaticMesh()->GetBounds().BoxExtent.X * 2;

	switch (DirectionToGet)
	{
	case AdjecantDirections::TopLeft:
		return GridPosition(ParentLocationX + halfSize, ParentLocationY - halfSize);
	case AdjecantDirections::TopCenter:
		return GridPosition(ParentLocationX + halfSize, ParentLocationY);
	case AdjecantDirections::TopRight:
		return GridPosition(ParentLocationX + halfSize, ParentLocationY + halfSize);
	case AdjecantDirections::Left:
		return GridPosition(ParentLocationX, ParentLocationY - halfSize);
	case AdjecantDirections::Right:
		return GridPosition(ParentLocationX, ParentLocationY + halfSize);
	case AdjecantDirections::BottomLeft:
		return GridPosition(ParentLocationX - halfSize, ParentLocationY - halfSize);
	case AdjecantDirections::BottomCenter:
		return GridPosition(ParentLocationX - halfSize, ParentLocationY);
	case AdjecantDirections::BottomRight:
		return GridPosition(ParentLocationX - halfSize, ParentLocationY + halfSize);
	}
	return GridPosition(0, 0);
}

void AdjacentCellsManager::GetAdjacentGridPos(GridPosition& GridPosition, AdjecantDirections DirectionToGet)
{
	const int GRID_COLUMNS = 10;
	const int GRID_ROWS = 10;
	int rowModifier = 0;
	int columnModifier = 0;
	int rowResult = 0;
	int columnResult = 0;

	switch (DirectionToGet)
	{
	case AdjecantDirections::TopLeft:
		rowModifier = -1;
		columnModifier = +1;
		break;
	case AdjecantDirections::TopCenter:
		columnModifier = +1;
		break;
	case AdjecantDirections::TopRight:
		columnModifier = +1;
		rowModifier = +1;
		break;
	case AdjecantDirections::Left:
		rowModifier = -1;
		break;
	case AdjecantDirections::Right:
		rowModifier = +1;
		break;
		case AdjecantDirections::BottomLeft:
		rowModifier = -1;
		columnModifier = -1;
		break;
	case AdjecantDirections::BottomCenter:
		columnModifier = -1;
		break;
	case AdjecantDirections::BottomRight:
		rowModifier = +1;
		columnModifier = -1;
		break;
	}
	rowResult = CellParent->Row + rowModifier;
	columnResult = CellParent->Column + columnModifier;

	if (rowResult > GRID_ROWS)
	{
		rowResult = 1;
	}
	else if (rowResult < 1)
	{
		rowResult = GRID_ROWS;
	}
	if (columnResult > GRID_COLUMNS)
	{
		columnResult = 1;
	}
	else if (columnResult < 1)
	{
		columnResult = GRID_COLUMNS;
	}

	//UE_LOG(LogTemp, Log, TEXT("%d : this column: %d AND this row: %d"), DirectionToGet, CellParent->Column, CellParent->Row);
	//UE_LOG(LogTemp, Log, TEXT("columnResult: %d AND rowResult: %d"), columnResult, rowResult);
	GridPosition.SetGridPos(rowResult, columnResult); // First Row (X) then Column (Y)
}

bool AdjacentCellsManager::RaycastAdjacentObjects(int posX, int posY, FHitResult& result, FVector componentUpVector,
                                                  UWorld* componentWorld, AdjecantDirections DirectionToGet )
{
	FVector StartRaycastLocation = FVector(posX, posY, 1000); // needs to be above the chunk collision 
	FVector DownwardVector = componentUpVector * -1;
	FVector EndLocation = StartRaycastLocation + DownwardVector * 2000;

	//UE_LOG(LogTemp, Log, TEXT("raycasting :) "));

	FHitResult HitResult;

	bool bHit = componentWorld->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation,
	                                                     ECC_GameTraceChannel3);

	if (bHit)
	{
		result = HitResult;
		if(DirectionToGet == AdjecantDirections::TopCenter)
		{
			//DrawDebugLine(componentWorld, StartRaycastLocation, EndLocation, FColor::Yellow);
		}
		else
		{
			//DrawDebugLine(componentWorld, StartRaycastLocation, EndLocation, FColor::Green);
		}
	}
	else
	{
		//DrawDebugLine(componentWorld, StartRaycastLocation, EndLocation, FColor::Red);
	}
	return bHit;
}