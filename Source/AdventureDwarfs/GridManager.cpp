// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Chunk.h"
#include "AdjecantDirections.h"
#include "AdjecantManager.h"
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
	GenerateGrid(10, 10);

}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridManager::GenerateGrid(int rows, int columns)
{
	SpawnChunk(0, 0,false);
	//SpawnChunk(1000, 0,false); // Chunks are 1000 units apart from one another.
	/*SpawnChunk(-1000, 0,true);
	SpawnChunk(0, 1000, true);
	SpawnChunk(0, -1000, true);
	
	SpawnChunk(1000, -1000, true);
	SpawnChunk(-1000, -1000, true);
	SpawnChunk(1000, 1000, true);
	SpawnChunk(-1000, 1000, true);*/
	InitializeCells();
}

AChunk* AGridManager::SpawnChunk(int posX, int posY, bool hidden)
{
	float randomChunkIndex = FMath::RandRange(0, ChunksLandforms.Num() - 1);
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

void AGridManager::ChunkStepped_Handler(AChunk* SteppedChunk) // TODO: Being called on every cell stepped. Maybe optimize this by making separate trigger for chunks?
{
	//UE_LOG(LogTemp, Log, TEXT("ChunkStepped_Handler: %s "),*SteppedChunk->GetName())
	SteppedChunk->SetAdjacents();
	SpawnChunksRecursive(SteppedChunk, 1);
	
}

void AGridManager::SpawnChunksRecursive(AChunk* startChunk, int depth)
{
	depth--;
	TArray<AChunk*> newChunks;
	for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
	{
		AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
		AChunk* ChunkToCheck = startChunk->AdjecantsManager->GetAdjacentObject(currentEnumValue);
		if (ChunkToCheck == nullptr) 
		{
			GridPosition posToSpawn = startChunk->AdjecantsManager->GetAdjacentPosition(currentEnumValue);
			//UE_LOG(LogTemp, Log, TEXT("Spawn Chunk at: X = %d  Y = %d"),posToSpawn.X, posToSpawn.Y)
			AChunk* newChunk = SpawnChunk(posToSpawn.X, posToSpawn.Y, false);
			startChunk->AdjecantsManager->SetAdjacent(currentEnumValue, newChunk);
			startChunk->InitializeCells();  // TODO: StartChunk, should have ONLY its side cells UPDATE their adjacent new chunk cells
			newChunks.Add(newChunk);
		}
	}
	for (AChunk* chunk : newChunks)
	{
		chunk->InitializeCells();
	}
}

void AGridManager::InitializeCells()
{
	for (AChunk* chunk : SpawnedChunks)
	{
		//chunk->InitializeCells();
	}
}

