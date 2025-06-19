// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include "AdjacentCellsManager.h"
#include "Cell.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Templates/SharedPointer.h"
#include "Curves/CurveFloat.h" // Spawning Animation needed CurveFloat and Timeline
#include "AdjecantManager.h"

#include "Engine/DataTable.h"
#include "ChunkDataField.h"
#include "ChunkUnit.h"
#include "Collectible.h"
#include "CollectibleDataAsset.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Character.h"

// r.LocalFogVolume.GlobalStartDistance=20  Command to make the local fog visible nearby

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; 
	

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;
	
	ChunkOverlapComponent = CreateDefaultSubobject<UBoxComponent>("ChunkColliderRoot");
	ChunkOverlapComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	ChunkOverlapComponent->SetRelativeLocation(FVector(0, 0, 150));
	ChunkOverlapComponent->SetBoxExtent(FVector(1000, 1000,1000 ));
	ChunkOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AChunk::ChunkStepped);
	ChunkOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &AChunk::ChunkLeft);
	ChunkOverlapComponent->SetupAttachment(RootComponent);
}

void AChunk::Construct()
{
	TArray<FChunkDataField*> CellsData;
	ChunkJsonData->GetAllRows<FChunkDataField>("", CellsData);
	
	UHierarchicalInstancedStaticMeshComponent* InstancedMeshComponent = NewObject<UHierarchicalInstancedStaticMeshComponent>(this," BASE CELL INSTANCE");
	InstancedMeshComponent->SetupAttachment(ChunkOverlapComponent);
	InstancedMeshComponent->SetStaticMesh(StaticMeshReference);
	InstancedMeshComponent->InstancingRandomSeed = FMath::Rand();
	InstancedMeshComponent->RegisterComponent();
	
	int counter=0;
	for (FChunkDataField* CellData : CellsData)
	{
		counter++;
		// Access data from Row as needed
		FVector Translation;
		Translation.X = CellData->translation[0];
		Translation.Y = CellData->translation[1];
		Translation.Z = CellData->translation[2];

		FRotator Rotation;
		Rotation.Roll = CellData->rotation[0];
		Rotation.Pitch = CellData->rotation[1];
		Rotation.Yaw = CellData->rotation[2];

		FVector Scale;
		Scale.X = CellData->scale[0];
		Scale.Y = CellData->scale[1];
		Scale.Z = CellData->scale[2];
		
		ConstructCell(counter, Translation, Rotation, Scale, InstancedMeshComponent, CellData->row, CellData->column);
	}
}

void AChunk::ConstructCell(int CellIndex, const FVector& Translation, const FRotator& Rotation, const FVector& Scale, UHierarchicalInstancedStaticMeshComponent* InstancedMeshComponent, int row, int column)
{
	// Creating Cell class instances:
	FString cellInstanceBaseName = "InstanceCell_";
	cellInstanceBaseName.AppendInt(CellIndex);
	const FName CellInstanceName(cellInstanceBaseName);
	UCell* Cell = NewObject<UCell>(this, CellInstanceName); // TODO: Maybe make this also instanced class OR a ordinary C++ class and not a unreal class UCell
	Cell->SetupAttachment(RootComponent);
	Cell->CellMesh = InstancedMeshComponent;	
	Cell->SetRelativeLocation(Translation);
	Cell->LocalLocation = Translation;	
	Cell->LocalRotation = Rotation;
	Cell->CellScale = Scale;

	FGridPosition CellPositionOnGrid(Translation.X, Translation.Y);
	CellPositionOnGrid.SetGridPos(row, column);  // TODO: I Have no IDEA why in-game the column and Row are reversed in this TMap.
	FChunkUnit newChunkUnit(CellPositionOnGrid); 
	AddChunkUnit(newChunkUnit, Cell);

	Cell->GridPosition = CellPositionOnGrid;
	Cell->Row = row;
	Cell->Column = column;
	Cell->ChunkParent = this;
	OnChunkStepped.AddUObject(Cell, &UCell::Raycast); 
	OnChunkLeft.AddUObject(Cell, &UCell::StopRaycast);
	Cell->RegisterComponent();
	ChunkCells.Add(Cell);
}

void AChunk::AddChunkUnit(FChunkUnit chunkUnit, UCell* cellToAdd)
{
	FChunkUnit* unit = ChunkUnits.FindByKey(chunkUnit);
	if(unit)
	{
		UE_LOG(LogTemp, Log, TEXT("AddChunkUnit"));
		unit->AddCell(cellToAdd);
	}
	else
	{
		chunkUnit.AddCell(cellToAdd);
		ChunkUnits.Add(chunkUnit);	
	}
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();	
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);
	AdjecantsManager = new AdjecantManager<AChunk>( 2000, Origin);

	//UE_LOG(LogTemp, Log, TEXT("BeginPlay of NEW CHUNK! "));
	//UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), Origin.X, Origin.Y,Origin.Z);
	//UE_LOG(LogTemp, Log, TEXT("current size is: x- %f,y- %f,z- %f"), BoxExtent.X,BoxExtent.Y,BoxExtent.Z);
}

void AChunk::Show()
{
	for (auto ChunkUnit : ChunkUnits)
	{
		for (auto cell : ChunkUnit.Cells)
		{
			cell->ShowCell();
		}
	}
}

void AChunk::SpawnCollectible(const TSubclassOf<ACollectible>& CollectibleToSpawn, UCollectibleDataAsset* data)
{
	float randomCellIndex = FMath::RandRange(0, ChunkCells.Num() - 1);
	UCell* chosenCell = ChunkCells[randomCellIndex];
	
	ACollectible* spawnedCollectible = GetWorld()->SpawnActor<ACollectible>(CollectibleToSpawn);
	spawnedCollectible->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	FBoxSphereBounds Bounds = chosenCell->Bounds;

	FVector Origin = Bounds.Origin;
	FVector BoxExtent = Bounds.BoxExtent;

	// Top is origin + Z extent
	FVector TopLocation = Origin + FVector(0, 0, BoxExtent.Z);
	spawnedCollectible->SetActorRelativeLocation(/*FVector(chosenCell->LocalLocation.X,chosenCell->LocalLocation.Y,chosenCell->LocalLocation.Z + 150)*/TopLocation); // +150 to have elevation above the cell
	spawnedCollectible->Init(data);
	
	if(data->Size.X > 1)
	{
		TArray<TPair<int,int>> pairs;
		
		for(int i=0; i < data->Size.X; i++)
		{
			int CellRowToPopulate = (i);		// TODO: Adjust depending on orientation default orientation NORTH:
			for(int k=0; k < data->Size.Y; k++)
			{
				int CellColumnToPopulate = (k);		// TODO: Adjust depending on orientation default orientation NORTH:
				if(CellColumnToPopulate== 0 && CellRowToPopulate==0)
					continue;

				pairs.Add(TPair<int,int>{CellColumnToPopulate,CellRowToPopulate});
			}
		}
		for (auto Pair : pairs)
		{
			//UE_LOG(LogTemp, Log, TEXT("column/row to populate : %d/%d"), Pair.Key,Pair.Value);
			TArray<UCell*> adjacentCell = chosenCell->AdjacentManager->GetAdjacentCell(Pair);
			if(adjacentCell.Num() > 0)
			{
				for (auto cell : adjacentCell)
				{
					cell->SetCollectible(spawnedCollectible, false);
					spawnedCollectible->ParentCells.Add(cell);
				}
			}
		}
	}
	
	chosenCell->SetCollectible(spawnedCollectible,true);
	spawnedCollectible->ParentCells.Add(chosenCell);
}

void AChunk::ChunkStepped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Handle the event
	//SteppedCell->ShowAdjacentCells(4, FloatCurve);
	// start raycasting each cell;
	//UE_LOG(LogTemp, Log, TEXT("ChunkStepped- other actor = %s"),*OtherActor->GetName());
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		OnChunkStepped.Broadcast(this);
		// This code will only execute if the OtherActor is the player pawn
		// You can put your functionality here
	}
	else
	{
		// This code will execute if the OtherActor is not the player pawn
	}	
}

void AChunk::ChunkLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		OnChunkLeft.Broadcast(this);
		UE_LOG(LogTemp, Log, TEXT("CHUNK LEFT BY PLAYER"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("CHUNK LEFT- other actor = %s"),*OtherActor->GetName());
	}	
}

TArray<UCell*> AChunk::GetCell(const FGridPosition& GridPosition) 
{
	FChunkUnit* FoundChunkUnit = ChunkUnits.FindByPredicate([&](const FChunkUnit& Unit)
	{
		return Unit.GridPosition == GridPosition;
	});
	
	if (FoundChunkUnit)
	{
		//UE_LOG(LogTemp, Log, TEXT("GetCell"));
		return FoundChunkUnit->Cells;
	}
	
	UE_LOG(LogTemp, Log, TEXT("NO CELLS FOUND"));
	return TArray<UCell*>();
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChunk::SetAdjacent()
{
	AdjecantsManager->SetAdjacentObjects(GetActorUpVector(), GetWorld());
}
