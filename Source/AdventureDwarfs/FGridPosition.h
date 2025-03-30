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
	int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Column;

	FGridPosition()
		: X(0), Y(0), Row(0), Column(0) {}

	FGridPosition(int32 PosX, int32 PosY)
		: X(PosX), Y(PosY), Row(0), Column(0) {}

	void SetGridPos(int32 InRow, int32 InColumn)
	{
		Row = InRow;
		Column = InColumn;
	}

	// Required operator== and hashing function for use in TMap
	bool operator==(const FGridPosition& Other) const
	{
		return Row == Other.Row && Column == Other.Column;
	}
};

// Required hashing function for use in TMap
FORCEINLINE uint32 GetTypeHash(const FGridPosition& GridPos)
{
	return HashCombine(GetTypeHash(GridPos.Row), GetTypeHash(GridPos.Column));
}
