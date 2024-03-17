// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Chunk.h"
#include "AdjecantDirections.h"
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
	SpawnChunk(1000, 0,false); // Chunks are 1000 units apart from one another.
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
	spawnedChunk->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	if (hidden)
	{
		spawnedChunk->Hide();
	}
	SpawnedChunks.Add(spawnedChunk);
	return spawnedChunk;
}

void AGridManager::InitializeCells()
{
	for (AChunk* chunk : SpawnedChunks)
	{
		chunk->InitializeCells();
	}
}

