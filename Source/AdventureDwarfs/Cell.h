// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "Cell.generated.h"


class AdjacentCellsManager;
DECLARE_MULTICAST_DELEGATE_OneParam(FCellEvent, UCell*);

enum class AdjecantDirections;
class GridPosition;
class UCurveFloat;

template<class T>
class AdjecantManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUREDWARFS_API UCell : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCell();
	
	AdjacentCellsManager* AdjacentManager;
	AChunk* ChunkParent;
	
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
	int32 Row;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	int32 Column;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FRotator LocalRotation;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PrintLocation();
	void ShowAdjacentCells(int depth);
	void ShowCell();

	UFUNCTION()
	void TimelineCallback(float Value);
	UFUNCTION()
	void TimelineFinishedCallback();

	void HideCell();
	void Raycast(AChunk* Chunk);
	void StopRaycast(AChunk* Chunk);

private:	
	FTimeline MyTimeline;
	bool activateRaycasting;
	bool CellProcessed;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
