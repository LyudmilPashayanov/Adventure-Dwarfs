// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Chunk.h"
#include "AdjecantDirections.h"
#include "AdjecantManager.h"
#include "Collectible.h"
#include "GridPosition.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid();
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridManager::GenerateGrid()
{
	SpawnChunk(0, 0,false);
}

AChunk* AGridManager::SpawnChunk(int posX, int posY, bool hidden)
{
	float randomChunkIndex = FMath::RandRange(0, ChunksLandforms.Num() - 1);
	//UE_LOG(LogTemp, Log, TEXT("SpawnActor posX: %d and posY: %d "),posX,posY)
	AChunk* spawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunksLandforms[randomChunkIndex], FVector(posX, posY, 0), FRotator().ZeroRotator);
	spawnedChunk->OnChunkStepped.AddUObject(this, &AGridManager::ChunkStepped_Handler);
	spawnedChunk->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	if (hidden == false)
	{
		spawnedChunk->Show();
	}
	SpawnedChunks.Add(spawnedChunk);
	return spawnedChunk;
}

void AGridManager::ChunkStepped_Handler(AChunk* SteppedChunk)
{
	//UE_LOG(LogTemp, Log, TEXT("ChunkStepped_Handler: %s "),*SteppedChunk->GetName())
	SteppedChunk->SetAdjacent();
	SpawnAdjacentChunks(SteppedChunk);
	
}

void AGridManager::SpawnAdjacentChunks(const AChunk* SteppedChunk)
{
	for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
	{
		AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
		AChunk* ChunkToCheck = SteppedChunk->AdjecantsManager->GetAdjacentObject(currentEnumValue);
		if (ChunkToCheck == nullptr) 
		{
			GridPosition posToSpawn = SteppedChunk->AdjecantsManager->GetAdjacentPosition(currentEnumValue);
			//UE_LOG(LogTemp, Log, TEXT("Spawn Chunk at: X = %d  Y = %d"),posToSpawn.X, posToSpawn.Y)
			AChunk* newChunk = SpawnChunk(posToSpawn.X, posToSpawn.Y, true);
			SteppedChunk->AdjecantsManager->SetAdjacent(currentEnumValue, newChunk);
			SetupCollectibles(newChunk);
		}
	}
}

void AGridManager::SetupCollectibles(AChunk* ChunkToSetup)
{
	ChunkToSetup->SpawnCollectible(BaseCollectible, CollectiblesData[0]);
}

