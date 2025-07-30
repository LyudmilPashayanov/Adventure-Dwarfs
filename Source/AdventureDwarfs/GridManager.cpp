// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

#include "AdjacentDirectionHelper.h"
#include "Chunk.h"
#include "AdjecantDirections.h"
#include "AdventureDwarfsCharacter.h"
#include "Cell.h"
#include "Kismet/GameplayStatics.h"

AGridManager::AGridManager()
{
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid();

	PlayerRevealRadius = UAdjacentDirectionHelper::GetSquareOffsets(15);

	AAdventureDwarfsCharacter* player = Cast<AAdventureDwarfsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player)
	{
		 player->OnPlayerPositionChanged.AddUObject(this, &AGridManager::RevealCellsAroundPlayer);
	}
}

void AGridManager::RevealCellsAroundPlayer(const FIntPoint& playerPosition)
{
	if (PlayerRevealRadius.Num() > 0)
	{
		for (FIntPoint Offset : PlayerRevealRadius)
		{	
			FIntPoint NeighborCoord = playerPosition + Offset;
			if (GlobalCellsMap2D.Contains(NeighborCoord))
			{
				for (UCell* cell : GlobalCellsMap2D[NeighborCoord])
				{
					cell->ShowCell();
				}
			}
		}
	}
}

void AGridManager::GenerateGrid()
{
	SpawnChunk(FIntPoint(0,0),false);
}

void AGridManager::AddCellToMap(FIntPoint coordinates2D, FIntVector coordinates3D, UCell* cell)
{
	GlobalCellsMap2D.FindOrAdd(coordinates2D).Add(cell);
	if (GlobalCellsMap3D.Contains(coordinates3D))
	{
		UE_LOG(LogTemp, Error, TEXT("A CELL WITH THAT COORDINATE ALREADY EXISTS, PLEASE CHECK WHY AND MAKE SURE EACH 3D coordinates is unique!"));
		return;
	}
	GlobalCellsMap3D.Add(coordinates3D, cell);
}

AChunk* AGridManager::SpawnChunk(FIntPoint position, bool hidden)
{
	if (GlobalChunkMap.Contains(position)) 
	{
		return nullptr;
	}
	
	float randomChunkIndex = FMath::RandRange(0, ChunksLandforms.Num() - 1);

	UE_LOG(LogTemp, Log, TEXT("CHUNK grid pos X: %d and grid pos Y : %d "),position.X, position.Y)

	AChunk* spawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunksLandforms[randomChunkIndex], FVector(position.X * 2000, position.Y * 2000, 0), FRotator().ZeroRotator);
	spawnedChunk->ChunkPosition =  position;
	spawnedChunk->OnChunkStepped.AddUObject(this, &AGridManager::ChunkStepped_Handler);
	spawnedChunk->Construct(this);
	if (hidden == false)
	{
		spawnedChunk->Show();
	}

	GlobalChunkMap.FindOrAdd(position) = spawnedChunk;
	
	return spawnedChunk;
}

void AGridManager::ChunkStepped_Handler(AChunk* SteppedChunk)
{
	//UE_LOG(LogTemp, Log, TEXT("ChunkStepped_Handler: %s "),*SteppedChunk->GetName())
	SpawnAdjacentChunks(SteppedChunk);
}

void AGridManager::SpawnAdjacentChunks(const AChunk* ChunkToSpawnAround)
{
	FIntPoint SpawnCenter  = ChunkToSpawnAround->ChunkPosition;
	for (EAdjacentDirection Dir : UAdjacentDirectionHelper::GetAllDirections())
	{
		FIntPoint Offset = UAdjacentDirectionHelper::GetOffset(Dir);
		FIntPoint NeighborCoord = SpawnCenter + Offset;
		
		AChunk* newChunk = SpawnChunk(NeighborCoord, true);
		if (newChunk)
		{
			SetupCollectibles(newChunk);
		}
	}
}

void AGridManager::SetupCollectibles(AChunk* ChunkToSetup)
{
	float randomCollectible = FMath::RandRange(0, CollectiblesData.Num() - 1);
	ChunkToSetup->SpawnCollectible(BaseCollectible, CollectiblesData[randomCollectible]);
}

