// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <deque>
#include "GridManager.generated.h"


class AChunk;

UCLASS()
class ADVENTUREDWARFS_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();
	//std::deque<std::deque<ACell*>> Grid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	//void OnCellEventReceived(ACell* Cell);
	AChunk* grid;

	void ChunkStepped_Handler(AChunk* SteppedChunk);
	void SpawnAdjacentChunks(const AChunk* SteppedChunk);
	void SetupCollectibles(AChunk* ChunkToSetup);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Cell Manager")
	TArray<TSubclassOf<class AChunk>> ChunksLandforms;
	void GenerateGrid();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute", meta = (AllowPrivateAccess = true))
	TArray<AChunk*> SpawnedChunks;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectibles Collection", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class ACollectible>> Collectibles;
	AChunk* SpawnChunk(int posX, int posY, bool hidden);
};
