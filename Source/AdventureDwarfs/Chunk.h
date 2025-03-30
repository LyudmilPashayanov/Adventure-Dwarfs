// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "FGridPosition.h"
#include "Engine/DataTable.h"
#include "Containers/Map.h"

#include "Chunk.generated.h"

struct FChunkUnit;
class UHierarchicalInstancedStaticMeshComponent;
class UCollectibleDataAsset;
class ACollectible;
class UCell;
struct ConstructorHelpers;
struct FGridPosition;
class UDataTable;
template<class T>
class AdjecantManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChunkStepped, AChunk*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChunkLeft, AChunk*);
UCLASS()

class ADVENTUREDWARFS_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:
	AChunk();
	void Construct();

	AdjecantManager<AChunk>* AdjecantsManager;
	FOnChunkStepped OnChunkStepped;
	FOnChunkLeft OnChunkLeft;

	// DataTable reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assets")
	UDataTable* ChunkJsonData;

	// StaticMesh reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assets")
	UStaticMesh* StaticMeshReference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	UBoxComponent* ChunkOverlapComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TArray<FChunkUnit> ChunkUnits;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TArray<UCell*> ChunkCells;
	UPROPERTY(EditAnywhere)
	UCurveFloat* FloatCurve;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void ConstructCell(int CellIndex, const FVector& Translation, const FRotator& Rotation, const FVector& Scale, UHierarchicalInstancedStaticMeshComponent* StaticMeshInstance, int row, int column);
	void AddChunkUnit(FChunkUnit gridPositionKey, UCell* cellToAdd);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ChunkStepped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ChunkLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	TArray<UCell*> GetCell(const FGridPosition& GridPosition);
	void SetAdjacent();
	void Show();
	void SpawnCollectible(const TSubclassOf<ACollectible>& CollectibleToSpawn, UCollectibleDataAsset* data );
};
