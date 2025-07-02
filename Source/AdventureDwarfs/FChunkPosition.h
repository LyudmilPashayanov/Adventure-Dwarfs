// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FChunkPosition.generated.h"

USTRUCT(BlueprintType)
struct ADVENTUREDWARFS_API FChunkPosition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ChunkCoordinates;
	
	FChunkPosition()
		: X(0), Y(0), ChunkCoordinates(0,0) {}

	FChunkPosition(int32 PosX, int32 PosY)
		: X(PosX), Y(PosY),  ChunkCoordinates(0,0) {}

	void SetChunkCoordinates(int32 InRow, int32 InColumn)
	{
		ChunkCoordinates.X = InRow;
		ChunkCoordinates.Y = InColumn;
	}

	// Required operator== and hashing function for use in TMap
	bool operator==(const FChunkPosition& Other) const
	{
		return ChunkCoordinates == Other.ChunkCoordinates;
	}
};

// Required hashing function for use in TMap
FORCEINLINE uint32 GetTypeHash(const FChunkPosition& GridPos)
{
	return GetTypeHash(GridPos.ChunkCoordinates);
}
