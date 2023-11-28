// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Cell.h"

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
	/*int posX = 0;
	int posY = 0;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t k = 0; k < columns; k++)
		{
			posX = (105 * i);
			posY = (105 * k);

			ACell* spawnedCell = SpawnCell(posX, posY);
			if (k == 0)
			{
				std::deque<ACell*> newRow;
				Grid.push_back(newRow);
			}
			Grid.back().push_back(spawnedCell);
		}
	}*/

	ACell* spawnedCell = SpawnCell(0, 0);
	
}

ACell* AGridManager::SpawnCell(int posX, int posY)
{
	ACell* spawnedCell = GetWorld()->SpawnActor<ACell>(CellPrefab, FVector(posX, posY, 0), FRotator().ZeroRotator);
	spawnedCell->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	spawnedCell->Init(posX, posY);
	spawnedCell->CellSteppedEvent.AddUObject(this, &AGridManager::OnCellEventReceived);
	return spawnedCell;
}

void AGridManager::OnCellEventReceived(ACell* SteppedCell)
{
	// Handle the event
	UE_LOG(LogTemp, Warning, TEXT("CellEvent received from Cell x= %d AND y= %d"), SteppedCell->posX, SteppedCell->posY);
	//if (SteppedCell->row == Grid.front().front()->row) //|| SteppedCell->row == Grid.front().back()->row)
	//{
	//	//Grid.front()[0]->SetActorLocation(FVector(SteppedCell->posX, SteppedCell->posY,50));
	//	ACell* newCell = SpawnCell(SteppedCell->row-1, SteppedCell->column, SteppedCell->posX-105, SteppedCell->posY);
	//	Grid.front().push_front(newCell);
	//}
	//else if (SteppedCell == Grid.front().back())
	//{
	//	//Grid.front()[0]->SetActorLocation(FVector(SteppedCell->posX, SteppedCell->posY,50));
	//	//ACell* newCell = SpawnCell(SteppedCell->posX, SteppedCell->posY + 105);
	//	//Grid.front().push_back(newCell);

	//}


}

