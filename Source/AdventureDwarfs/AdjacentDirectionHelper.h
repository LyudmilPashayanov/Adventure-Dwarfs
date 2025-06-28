// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdjacentDirectionHelper.generated.h"


enum class EAdjacentDirection : uint8;

UCLASS()
class ADVENTUREDWARFS_API UAdjacentDirectionHelper : public UObject
{
	GENERATED_BODY()

public:
	// Get all directions
	static TArray<EAdjacentDirection> GetAllDirections();

	// Get all directions except center (optional)
	static TArray<FIntPoint> GetAllDirectionOffsets();

	// Get offset from enum
	static FIntPoint GetOffset(EAdjacentDirection Direction);

	static TArray<FIntPoint> GetSquareOffsets(int32 Radius, bool bIncludeCenter = false);

};
