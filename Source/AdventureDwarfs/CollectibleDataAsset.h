// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CollectibleDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREDWARFS_API UCollectibleDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Orientation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector Size;
};
