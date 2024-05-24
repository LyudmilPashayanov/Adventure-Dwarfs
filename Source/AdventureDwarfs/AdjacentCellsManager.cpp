// Fill out your copyright notice in the Description page of Project Settings.

#include "AdjacentCellsManager.h"

#include <utility>

#include "Components/HierarchicalInstancedStaticMeshComponent.h"

AdjacentCellsManager::AdjacentCellsManager(const UCell* ParentCell)
{
	CellParent = ParentCell;
}

void AdjacentCellsManager::ShowAdjacentCells(int depth)
{
	TArray<TPair<int, int>>  combinations; // TODO: This has to be created ones, and can be reused after, if Depth doesn't change.
	for (int col = -depth; col <= depth; ++col) {
		for (int row = -depth; row <= depth; ++row) {
			combinations.Push(TPair<int, int>(col,row));
		}
	}
	
	//UE_LOG(LogTemp, Log, TEXT("this cell column/row : %d/%d"), CellParent->Column,CellParent->Row);

	for (auto Combination : combinations)
	{
		//UE_LOG(LogTemp, Log, TEXT("Combination column/row to GetAdjacentCell : %d/%d"), Combination.Key,Combination.Value);

		GetAdjacentCell(Combination)->ShowCell();
	}
}

UCell* AdjacentCellsManager::GetAdjacentCell(TPair<int,int> colRowPair)
{
	FHitResult Hit;
	GridPosition positionToCheck = GetAdjacentCellLocation(colRowPair);
	if (RaycastAdjacentObjects(positionToCheck.X, positionToCheck.Y, Hit))
	{
		UCell* cell = Cast<AChunk>(Hit.GetActor())->GetCell((positionToCheck));
		return cell;
	}
	else
	{
		// Spawn a chunk here and get the cell again.
			
		/*CellParent->ChunkParent->OnChunkStepped.Broadcast(CellParent->ChunkParent);
		GridPosition positionToCheck = GetAdjacentCellLocation(colRowPair);
		if (RaycastAdjacentObjects(positionToCheck.X, positionToCheck.Y, Hit))
		{
			UCell* cell = Cast<AChunk>(Hit.GetActor())->GetCell((positionToCheck));
			return cell;
		}*/
		UE_LOG(LogTemp, Log, TEXT("ERROR ERROR RETURNING NULLPTR CELL- Check this code path !!!!"));
		UE_LOG(LogTemp, Log, TEXT("positionToCheck was X:%d and Y:%d:"),positionToCheck.X,positionToCheck.Y);
	}

	return nullptr;
}

GridPosition AdjacentCellsManager::GetAdjacentCellLocation(const TPair<int, int>  RowColumnPair) const
{
	const int GRID_COLUMNS = 10;
	const int GRID_ROWS = 10;

	const int halfSize = CellParent->CellMesh->GetStaticMesh()->GetBounds().BoxExtent.X * 2;
	const int ParentLocationX = CellParent->GetComponentLocation().X + (halfSize * RowColumnPair.Value); // maybe has to be switched with bottom
	const int ParentLocationY = CellParent->GetComponentLocation().Y + (halfSize * RowColumnPair.Key); // maybe has to be switched with top
	
	GridPosition resultPosition = GridPosition(ParentLocationX , ParentLocationY);
	int ColumnResult = CellParent->Column + RowColumnPair.Key;
	int RowResult = CellParent->Row + RowColumnPair.Value;
	if(RowResult > GRID_ROWS)
	{
		RowResult = RowResult - GRID_ROWS;
	}
	else if(RowResult < 1)
	{
		RowResult = GRID_ROWS + RowResult;
	}
	
	if(ColumnResult > GRID_COLUMNS)
	{
		ColumnResult = ColumnResult - GRID_COLUMNS;
	}
	else if(ColumnResult < 1) 
	{
		ColumnResult = GRID_COLUMNS + ColumnResult;
	}
//UE_LOG(LogTemp, Log, TEXT("COUNTER: %d"), counter);
//UE_LOG(LogTemp, Log, TEXT("pair: col: %d, row: %d"), RowColumnPair.Key, RowColumnPair.Value);
//UE_LOG(LogTemp, Log, TEXT("Gettings Cell with RowResult: %d AND ColumnResult result: %d"), RowResult, ColumnResult);

//UE_LOG(LogTemp, Log, TEXT("Current CELL X = %f AND Y: %f"), CellParent->GetComponentLocation().X, CellParent->GetComponentLocation().Y);
//UE_LOG(LogTemp, Log, TEXT("Getting Position X = %d AND Y: %d"), ParentLocationX, ParentLocationY);
//UE_LOG(LogTemp, Log, TEXT("-------------------------"));

	resultPosition.SetGridPos(RowResult,ColumnResult);
	return resultPosition;
}

bool AdjacentCellsManager::RaycastAdjacentObjects(int posX, int posY, FHitResult& result)
{
	FVector StartRaycastLocation = FVector(posX, posY, 1000); // needs to be above the chunk collision 
	FVector DownwardVector = CellParent->GetOwner()->GetActorUpVector() * -1;
	FVector EndLocation = StartRaycastLocation + DownwardVector * 2000;

	//UE_LOG(LogTemp, Log, TEXT("raycasting :) "));

	FHitResult HitResult;

	bool bHit = CellParent->GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation,
	                                                     ECC_GameTraceChannel3);

	if (bHit)
	{
		result = HitResult;
	}
	else
	{
		DrawDebugLine(CellParent->GetWorld(), StartRaycastLocation, EndLocation, FColor::Red,false,5); 
	}
	return bHit;
}