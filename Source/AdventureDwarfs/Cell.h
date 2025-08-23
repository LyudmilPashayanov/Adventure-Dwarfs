// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.h"
#include "Cell.generated.h"

class ADebugTextActor;
DECLARE_MULTICAST_DELEGATE_OneParam(FCellEvent, UCell*);

struct FChunkPosition;
class UCurveFloat;
class UCollectibleDataAsset;

UCLASS()
class ADVENTUREDWARFS_API UCell : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	ADebugTextActor* DebugActor;
public:	
	// Sets default values for this component's properties
	UCell();
	void PrintLocation() const;
	void ShowCell();
	void HideCell();
	void SetCollectible(ACollectible* Collectible, bool IsMainParent);
	void InitTransform(const FVector& LocalPosition, const FVector2D& WorldPosition, const FRotator& Rotation, const FVector& Scale);
	bool IsWalkable();
	void Highlight(int number);
	void ShowPathfindingDebugData(FString debugData);

	FVector CellLocalSurface;
	FVector WorldCellSurface;
	
	static constexpr ECollisionChannel TraceChannelValue = ECC_GameTraceChannel1; // Custom trace channel

	FCellEvent CellSteppedEvent;

	bool ShouldRaycast;
	bool IsCellVisible = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	UInstancedStaticMeshComponent* CellMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FIntVector Coordinates;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	AChunk* ChunkParent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	ACollectible* SpawnedCollectible;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int CellMeshIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FVector OriginCenterLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int32 RowInChunk;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int32 ColumnInChunk;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int32 Height;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FRotator LocalRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FVector CellWorldSurface;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FVector CellScale;
	
private:
	void SetLocation(const FVector& Location, const FVector2D& WorldPosition);

	bool IsMainCollectibleParent;
	int FrameCounter=0;
};
