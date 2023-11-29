// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Cell.h"
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

void AGridManager::GenerateGrid(int rows, int columns)
{
	ACell* spawnedCell = SpawnCell(0, 0);
}

ACell* AGridManager::SpawnCell(int posX, int posY)
{
	ACell* spawnedCell = GetWorld()->SpawnActor<ACell>(CellPrefab, FVector(posX, posY, 0), FRotator().ZeroRotator);
	spawnedCell->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	spawnedCell->Init(GridPosition(posX,posY));
	spawnedCell->CellSteppedEvent.AddUObject(this, &AGridManager::OnCellEventReceived);
	return spawnedCell;
}

void AGridManager::OnCellEventReceived(ACell* SteppedCell)
{
	// Handle the event
	//UE_LOG(LogTemp, Warning, TEXT("CellEvent received from Cell x= %d AND y= %d"), SteppedCell->posX, SteppedCell->posY);
	for (int i = 0; i < static_cast<int>(AdjecentDirections::Count); ++i) {
		AdjecentDirections currentEnumValue = static_cast<AdjecentDirections>(i);

		if (!SteppedCell->CheckAdjecentCell(currentEnumValue))
		{
			GridPosition pos = SteppedCell->GetAdjecentPosition(currentEnumValue);
			SpawnCell(pos.X, pos.Y);
		}
	}
	
}

