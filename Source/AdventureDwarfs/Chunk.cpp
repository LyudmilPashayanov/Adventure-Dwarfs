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
#include "Collectible.h"
#include "CollectibleDataAsset.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Character.h"

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
		Translation.X = CellData->translation[0]; //- 450;
		Translation.Y = CellData->translation[1]; //+ 440;
		Translation.Z = CellData->translation[2];

		FRotator Rotation;
		Rotation.Roll = CellData->rotation[0];
		Rotation.Pitch = CellData->rotation[1];
		Rotation.Yaw = CellData->rotation[2];
			
		ConstructCell(counter, Translation, Rotation, InstancedMeshComponent, CellData->row, CellData->column);
	}
}

void AChunk::ConstructCell(int CellIndex, const FVector& Translation, const FRotator& Rotation, UHierarchicalInstancedStaticMeshComponent* InstancedMeshComponent, int row, int column)
{
	// Creating Cell class instances:
	FString cellInstanceBaseName = "InstanceCell_";
	cellInstanceBaseName.AppendInt(CellIndex);
	const FName CellInstanceName(cellInstanceBaseName);
	UCell* Cell = NewObject<UCell>(this, CellInstanceName); // TODO: Maybe make this also instanced class OR a ordinary C++ class and not a unreal class UCel
	Cell->SetupAttachment(RootComponent);
	Cell->CellMesh = InstancedMeshComponent;	
	Cell->SetRelativeLocation(Translation);
	Cell->LocalLocation = Translation;	
	Cell->LocalRotation = Rotation;
	Cell->Row = row;
	Cell->Column = column;
	Cell->ChunkParent = this;
	OnChunkStepped.AddUObject(Cell, &UCell::Raycast); 
	OnChunkLeft.AddUObject(Cell, &UCell::StopRaycast);
	Cell->RegisterComponent();
	ChunkCells.Add(Cell);
	LocationCellPairs.Add(FString::Format(TEXT("{0}-{1}"), { row, column }), Cell);	// TODO: I Have no IDEA why in-game the column and Row are reversed in this TMap.
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
	for (auto Element : LocationCellPairs)
	{
		Element.Value->ShowCell();
	}
}

void AChunk::SpawnCollectible(const TSubclassOf<ACollectible>& CollectibleToSpawn, UCollectibleDataAsset* data)
{
	float randomCellIndex = FMath::RandRange(0, ChunkCells.Num() - 1);
	UCell* chosenCell = ChunkCells[randomCellIndex];
	
	ACollectible* spawnedCollectible = GetWorld()->SpawnActor<ACollectible>(CollectibleToSpawn);
	spawnedCollectible->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	spawnedCollectible->SetActorRelativeLocation(FVector(chosenCell->LocalLocation.X,chosenCell->LocalLocation.Y,chosenCell->LocalLocation.Z + 150)); // +150 to have elevation above the cell
	spawnedCollectible->Init(data);
	TArray<TPair<int,int>> pairs;
	
	if(data->Size.X > 1)
	{
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
			UCell* adjacentCell = chosenCell->AdjacentManager->GetAdjacentCell(Pair);
			if(adjacentCell)
			{
				adjacentCell->SetCollectible(spawnedCollectible, false);
				spawnedCollectible->ParentCells.Add(adjacentCell);
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

UCell* AChunk::GetCell(const GridPosition& GridPosition) // TODO: Make the LocationCellPair, to work with GridPosition and not with a string.
{
	FString key = FString::Format(TEXT("{0}-{1}"),{ GridPosition.Row, GridPosition.Column }); //TODO: I Have no IDEA why the column and Row are reversed in this TMap.
	return LocationCellPairs[key];
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
