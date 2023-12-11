// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Chunk.h"
#include "AdjecentDirections.h"
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

// GRID ARE 945 units apart from one another.
void AGridManager::GenerateGrid(int rows, int columns)
{
	SpawnChunk(0, 0);
	SpawnChunk(945, 0);
	SpawnChunk(-945, 0);
	SpawnChunk(0, 945);
	SpawnChunk(0, -945);
}

AChunk* AGridManager::SpawnChunk(int posX, int posY)
{
	AChunk* spawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkFlat_Prefab, FVector(posX, posY, 0), FRotator().ZeroRotator);
	spawnedChunk->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	SpawnedChunks.Add(spawnedChunk);
	//spawnedChunk->Init(GridPosition(posX,posY));
	//spawnedChunk->CellSteppedEvent.AddUObject(this, &AGridManager::OnCellEventReceived);
	return spawnedChunk;
}

//void AGridManager::OnCellEventReceived(ACell* SteppedCell)
//{
//	// Handle the event
//	//UE_LOG(LogTemp, Warning, TEXT("CellEvent received from Cell x= %d AND y= %d"), SteppedCell->posX, SteppedCell->posY);
//	/*for (int i = 0; i < static_cast<int>(AdjecentDirections::Count); ++i) {
//		AdjecentDirections currentEnumValue = static_cast<AdjecentDirections>(i);
//
//		if (!SteppedCell->CheckAdjecentCell(currentEnumValue))
//		{
//			GridPosition pos = SteppedCell->GetAdjecentPosition(currentEnumValue);
//			SpawnCell(pos.X, pos.Y);
//		}
//	}*/
//}

