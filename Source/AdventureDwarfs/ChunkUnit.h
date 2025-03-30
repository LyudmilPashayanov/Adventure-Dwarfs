// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGridPosition.h"
#include "ChunkUnit.generated.h"

class UCell;

USTRUCT(BlueprintType)
struct ADVENTUREDWARFS_API FChunkUnit
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridPosition GridPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCell*> Cells;  

	FChunkUnit() {}

	FChunkUnit(FGridPosition& InGridPosition)
		: GridPosition(InGridPosition) {}

	void AddCell(UCell* cellToAdd)
	{
		if(Cells.Num() > 0)
		{
			Cells.Add(cellToAdd);
		}
		else
		{
			Cells = TArray<UCell*>{cellToAdd};
		}
	}
	
	// Comparison operator for TSet and TMap
	bool operator==(const FChunkUnit& Other) const
	{
		return GridPosition == Other.GridPosition; 
	}
};

// Hash function for FChunkUnit (needed for using it as a key in TSet or as a map key in a TMap)
FORCEINLINE uint32 GetTypeHash(const FChunkUnit& ChunkUnit)
{
	return GetTypeHash(ChunkUnit.GridPosition);
}