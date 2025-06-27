// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGridPosition.generated.h"

USTRUCT(BlueprintType)
struct ADVENTUREDWARFS_API FGridPosition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint LocalGridPosition;
	
	FGridPosition()
		: X(0), Y(0), LocalGridPosition(0,0) {}

	FGridPosition(int32 PosX, int32 PosY)
		: X(PosX), Y(PosY),  LocalGridPosition(0,0) {}

	void SetGridPos(int32 InRow, int32 InColumn)
	{
		LocalGridPosition.X = InRow;
		LocalGridPosition.Y = InColumn;
	}

	// Required operator== and hashing function for use in TMap
	bool operator==(const FGridPosition& Other) const
	{
		return LocalGridPosition == Other.LocalGridPosition;
	}
};

// Required hashing function for use in TMap
FORCEINLINE uint32 GetTypeHash(const FGridPosition& GridPos)
{
	return GetTypeHash(GridPos.LocalGridPosition);
}
