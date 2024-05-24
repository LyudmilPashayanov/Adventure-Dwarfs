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
	PrimaryActorTick.bCanEverTick = true; 
	

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;
	
	ChunkOverlapComponent = CreateDefaultSubobject<UBoxComponent>("ChunkColliderRoot");
	ChunkOverlapComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	ChunkOverlapComponent->SetRelativeLocation(FVector(0, 0, 150));
	ChunkOverlapComponent->SetBoxExtent(FVector(490, 490,490 ));
	ChunkOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AChunk::ChunkStepped);
	ChunkOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &AChunk::ChunkLeft);
	ChunkOverlapComponent->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UDataTable> JsonConstructData = GetGridConstructJsonPath();
	if (JsonConstructData.Succeeded())
	{
		TArray<FChunkDataField*> CellsData;
		JsonConstructData.Object->GetAllRows<FChunkDataField>("", CellsData);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>CellMeshAsset(TEXT("StaticMesh'/Game/LyudmilContent/Chunks/default_cell.default_cell'")); // Get the building mesh - cell

		UHierarchicalInstancedStaticMeshComponent* InstancedMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(" BASE CELL INSTANCE");
		InstancedMeshComponent->SetupAttachment(ChunkOverlapComponent);
		InstancedMeshComponent->SetStaticMesh(CellMeshAsset.Object);
		InstancedMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		InstancedMeshComponent->InstancingRandomSeed = FMath::Rand();
		int counter=0;
		for (FChunkDataField* Cell : CellsData)
		{
			counter++;
			// Access data from Row as needed
			FVector Translation;
			Translation.X = Cell->translation[0]; //- 450;
			Translation.Y = Cell->translation[1]; //+ 440;
			Translation.Z = Cell->translation[2];

			FRotator Rotation;
			Rotation.Roll = Cell->rotation[0];
			Rotation.Pitch = Cell->rotation[1];
			Rotation.Yaw = Cell->rotation[2];
			
		
			// Set up overlap events for the component

			ConstructCell(counter, Translation, Rotation, InstancedMeshComponent, Cell->column, Cell->row); // TODO: ROW AND COLUMN IS REVERSED IN THE JSON DATA
		}
	}
	else
	{
	 	UE_LOG(LogTemp, Log, TEXT("Failed to fetch JSON data table needed for constructing the chunk."));
	}
}

void AChunk::ConstructCell(int CellIndex, const FVector& Translation, const FRotator& Rotation, UHierarchicalInstancedStaticMeshComponent* InstancedMeshComponent, int row, int column)
{
	// Creating Cell class instances:
	FString cellInstanceBaseName = "InstanceCell_";
	cellInstanceBaseName.AppendInt(CellIndex);
	const FName CellInstanceName(cellInstanceBaseName);
	UCell* Cell = CreateDefaultSubobject<UCell>(CellInstanceName); // TODO: Maybe make this also instanced class OR a ordinary C++ class and not a unreal class UCell
	Cell->SetupAttachment(RootComponent);
	Cell->CellMesh = InstancedMeshComponent;	
	Cell->SetWorldLocation(Translation);
	Cell->LocalLocation = Translation;	
	Cell->LocalRotation = Rotation;
	Cell->Row = row;
	Cell->Column = column;
	Cell->ChunkParent = this;
	OnChunkStepped.AddUObject(Cell, &UCell::Raycast); 
	OnChunkLeft.AddUObject(Cell, &UCell::StopRaycast);
	ChunkCells.Add(Cell);
	LocationCellPairs.Add(FString::Format(TEXT("{0}-{1}"), { row, column }), Cell);	// TODO: I Have no IDEA why in-game the column and Row are reversed in this TMap.
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

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	
	Super::BeginPlay();
	FVector Origin;
	FVector BoxExtent;
	
	GetActorBounds(false, Origin, BoxExtent);
	AdjecantsManager = new AdjecantManager<AChunk>( 1000, Origin);

	//UE_LOG(LogTemp, Log, TEXT("BeginPlay of NEW CHUNK! "));
	//UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), Origin.X, Origin.Y,Origin.Z);
	//UE_LOG(LogTemp, Log, TEXT("current size is: x- %f,y- %f,z- %f"), BoxExtent.X,BoxExtent.Y,BoxExtent.Z);
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

UCell* AChunk::GetCell(const GridPosition& GridPosition)
{
	FString key = FString::Format(TEXT("{0}-{1}"),{ GridPosition.Column, GridPosition.Row }); //TODO: I Have no IDEA why the column and Row are reversed in this TMap.
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

ConstructorHelpers::FObjectFinder<UDataTable> AChunk::GetGridConstructJsonPath()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinderHill(TEXT("/Script/Engine.DataTable'/Game/LyudmilContent/ChunksJSON/chunk_data_HillChunk.chunk_data_HillChunk'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinderFlat(TEXT("/Script/Engine.DataTable'/Game/LyudmilContent/ChunksJSON/chunk_data_FlatChunk.chunk_data_FlatChunk'"));

	TArray<FString> Substrings;
	GetName().ParseIntoArray(Substrings, TEXT("_"), true);
	FString blueprintName;
	for (int i = 0; i < Substrings.Num(); i++)
	{
		if (Substrings[i] == "BP")
		{
			blueprintName = Substrings[i + 1];
			break;
		}
	}

	if (blueprintName == "HillChunk")
	{
		return DataTableFinderHill;
	}
	else if (blueprintName == "FlatChunk")
	{
		return DataTableFinderFlat;
	}
	
	return DataTableFinderFlat;
}

