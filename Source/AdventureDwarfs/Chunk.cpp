// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include "Cell.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Templates/SharedPointer.h"
#include "Curves/CurveFloat.h" // Spawning Animation needed CurveFloat and Timeline
#include "AdventureDwarfsCharacter.h"
#include "Engine/DataTable.h"
#include "ChunkDataField.h"
#include "ChunkUnit.h"
#include "Collectible.h"
#include "CollectibleDataAsset.h"
#include "GridManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
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
	ChunkOverlapComponent->SetupAttachment(RootComponent);
	ChunkOverlapComponent->SetRelativeLocation(FVector(0, 0, 150));
	ChunkOverlapComponent->SetBoxExtent(FVector(1000, 1000,1000 ));
	ChunkOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AChunk::ChunkStepped);
	ChunkOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &AChunk::ChunkLeft);
}

void AChunk::Construct(AGridManager* gridManager)
{
	GridManager = gridManager;
	TArray<FChunkDataField*> CellsData;
	ChunkJsonData->GetAllRows<FChunkDataField>("", CellsData);
	
	UHierarchicalInstancedStaticMeshComponent* InstancedMeshComponent = NewObject<UHierarchicalInstancedStaticMeshComponent>(this," BASE CELL INSTANCE");
	InstancedMeshComponent->SetupAttachment(RootComponent);
	InstancedMeshComponent->SetStaticMesh(StaticMeshReference);
	InstancedMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InstancedMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	InstancedMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
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

void AChunk::ConstructCell(int CellIndex, const FVector& CellLocalTranslation, const FRotator& Rotation, const FVector& Scale, UHierarchicalInstancedStaticMeshComponent* InstancedMeshComponent, int chunkRow, int ChunkColumn)
{
	FString cellInstanceBaseName = "InstanceCell_";
	cellInstanceBaseName.AppendInt(CellIndex);
	const FName CellInstanceName(cellInstanceBaseName);
	
	UCell* Cell = NewObject<UCell>(this, CellInstanceName);
	
	Cell->CellMesh = InstancedMeshComponent;
	
	Cell->InitTransform(CellLocalTranslation, Rotation, Scale);

	FChunkPosition LocalChunkPosition(CellLocalTranslation.X, CellLocalTranslation.Y);
	LocalChunkPosition.SetChunkCoordinates(chunkRow, ChunkColumn);
	FChunkUnit newChunkUnit(LocalChunkPosition);
	AddChunkUnit(newChunkUnit, Cell);

	Cell->RowInChunk = chunkRow;
	Cell->ColumnInChunk = ChunkColumn;
	Cell->ChunkParent = this;
	
	FIntPoint GlobalCellPosition2D((ChunkPosition.X * 20) + chunkRow, (ChunkPosition.Y * 20) + ChunkColumn);
	FIntVector GlobalCellPosition3D(GlobalCellPosition2D.X, GlobalCellPosition2D.Y, Cell->Height);
	Cell->Coordinates = GlobalCellPosition3D;
	GridManager->AddCellToMap(GlobalCellPosition2D, GlobalCellPosition3D, Cell);
	bool debug = true;
	if (debug)
	{
		UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(this);
		TextComponent->RegisterComponent();
		TextComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		TextComponent->SetHorizontalAlignment(EHTA_Center);
		TextComponent->SetTextRenderColor(FColor::Green);
		TextComponent->SetWorldSize(15.0f); // Font size
		TextComponent->SetRelativeLocation(FVector(CellLocalTranslation.X, CellLocalTranslation.Y, Cell->CellSurface.Z));
		FString Text = FString::Printf(TEXT("%d, %d, %d"), GlobalCellPosition2D.X, GlobalCellPosition2D.Y, Cell->Height);
		TextComponent->SetText(FText::FromString(Text));

		//ACollectible* spawnedCollectible = GetWorld()->SpawnActor<ACollectible>(GridManager->BaseCollectible);
		//spawnedCollectible->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
//
		//spawnedCollectible->SetActorRelativeLocation(FVector(Cell->LocalLocation.X, Cell->LocalLocation.Y, Cell->CellSurface.Z+150));
		//spawnedCollectible->Init(GridManager->CollectiblesData[0]);
	}
}

void AChunk::AddChunkUnit(FChunkUnit chunkUnit, UCell* cellToAdd)
{
	FChunkUnit* unit = ChunkUnits.FindByKey(chunkUnit);
	if(unit)
	{
		//UE_LOG(LogTemp, Log, TEXT("AddChunkUnit"));
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
	float randomCoordinationIndex = FMath::RandRange(0, ChunkUnits.Num() - 1);
	float randomCellIndex = FMath::RandRange(0, ChunkUnits[randomCoordinationIndex].Cells.Num() - 1);
	UCell* chosenCell = ChunkUnits[randomCoordinationIndex].Cells[randomCellIndex];
	
	ACollectible* spawnedCollectible = GetWorld()->SpawnActor<ACollectible>(CollectibleToSpawn);
	spawnedCollectible->SetActorLocation(chosenCell->CellSurface);

	spawnedCollectible->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	spawnedCollectible->Init(data);
	
	if(data->Size.X > 1)
	{
		TArray<TPair<int,int>> pairs;
		
		for(int i=0; i < data->Size.X; i++)
		{
			int CellRowToPopulate = i;		// TODO: Adjust depending on orientation default orientation NORTH:
			for(int k=0; k < data->Size.Y; k++)
			{
				int CellColumnToPopulate = k;		// TODO: Adjust depending on orientation default orientation NORTH:
				if (CellColumnToPopulate== 0 && CellRowToPopulate==0)
					continue;

				pairs.Add(TPair<int,int>{CellColumnToPopulate,CellRowToPopulate});
			}
		}
		for (auto Pair : pairs)
		{
			//UE_LOG(LogTemp, Log, TEXT("column/row to populate : %d/%d"), Pair.Key,Pair.Value);

			FIntPoint NeighborCoord = FIntPoint(chosenCell->Coordinates.X + Pair.Key, chosenCell->Coordinates.Y + Pair.Value);
			if (GridManager->GlobalCellsMap2D.Contains(NeighborCoord))
			{
				for (UCell* cell : GridManager->GlobalCellsMap2D[NeighborCoord])
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
	AAdventureDwarfsCharacter* PlayerCharacter = Cast<AAdventureDwarfsCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		OnChunkStepped.Broadcast(this);
		PlayerCharacter->CurrentChunk = this;
		//UE_LOG(LogTemp, Log, TEXT("NEW CHUNK REGISTERED!"));

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
		//UE_LOG(LogTemp, Log, TEXT("CHUNK LEFT BY PLAYER"));
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("CHUNK LEFT- other actor = %s"),*OtherActor->GetName());
	}	
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}