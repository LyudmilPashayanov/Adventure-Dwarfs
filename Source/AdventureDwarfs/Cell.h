// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.h"
#include "Cell.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCellEvent, UCell*);

struct FChunkPosition;
class UCurveFloat;
class UCollectibleDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUREDWARFS_API UCell : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCell();
	
	AChunk* ChunkParent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	ACollectible* SpawnedCollectible;
	static constexpr ECollisionChannel TraceChannelValue = ECC_GameTraceChannel1; // Custom trace channel

	FCellEvent CellSteppedEvent;
	bool ShouldRaycast;
	bool IsCellVisible = false;
	UHierarchicalInstancedStaticMeshComponent* CellMesh; //TODO: Clear out pointer afterwards destroying of object

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int CellMeshIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FVector LocalLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FChunkPosition GridPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int32 RowInChunk;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int32 ColumnInChunk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FRotator LocalRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FVector CellScale;
	
	void PrintLocation() const;
	void ShowCell();

	void HideCell();
	void Raycast(AChunk* Chunk);
	void StopRaycast(AChunk* Chunk);
	void SetCollectible(ACollectible* Collectible, bool IsMainParent);

private:	
	bool activateRaycasting;
	bool CellProcessed;
	bool IsMainCollectibleParent;
	int FrameCounter=0;
};
