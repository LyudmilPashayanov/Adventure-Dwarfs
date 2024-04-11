// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "GridPosition.h"
#include "Engine/DataTable.h"
#include "Containers/Map.h"

#include "Chunk.generated.h"

class UCell;
struct ConstructorHelpers;
class FObjectFinder;
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
	
	AdjecantManager<AChunk>* AdjecantsManager;
	FOnChunkStepped OnChunkStepped;
	FOnChunkLeft OnChunkLeft;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	UBoxComponent* ChunkOverlapComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TMap<FString, UCell*> LocationCellPairs;
	UPROPERTY(EditAnywhere)
	UCurveFloat* FloatCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSoftObjectPtr<UDataTable> ChunkJsonData;

	UFUNCTION()
	void ChunkStepped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ChunkLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void ShowCell(const GridPosition& GridPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual ConstructorHelpers::FObjectFinder<UDataTable> GetGridConstructJsonPath();
private:
	void ConstructCell(int CellIndex, const FVector& Translation, const FRotator& Rotation, UHierarchicalInstancedStaticMeshComponent* StaticMeshInstance, int row, int column);
	void OnCellStepped(UCell* Cell);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitializeCells();
	void SetAdjacents();
	void Show();
};
