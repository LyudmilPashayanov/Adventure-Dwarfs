// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "AdjecentDirections.h"
#include "Cell.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Templates/SharedPointer.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "Engine/DataTable.h"
#include "ChunkDataField.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	ConstructorHelpers::FObjectFinder<UDataTable> toUse = GetGridConstructJsonPath();
	if (toUse.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("FILE EXISTS: %s"), *toUse.Object->GetName());
	
		TArray<FChunkDataField*> CellsData;
		toUse.Object->GetAllRows<FChunkDataField>("", CellsData);
		for (FChunkDataField* Cell : CellsData)
		{
			// Access data from Row as needed
			FVector Translation;
			Translation.X = Cell->translation[0] - 450;
			Translation.Y = Cell->translation[1] + 440;
			Translation.Z = Cell->translation[2];

			FRotator Rotation;
			Rotation.Roll = Cell->rotation[0];
			Rotation.Pitch = Cell->rotation[1];
			Rotation.Yaw = Cell->rotation[2];
			ConstructCell(Cell->cell_index, Translation, Rotation);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("FAILED FAILED FAILED "));
	}
}

void AChunk::ConstructCell(int CellIndex, FVector Translation, FRotator Rotation)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CellMeshAsset(TEXT("StaticMesh'/Game/LyudmilContent/Chunks/default_cell.default_cell'")); // Get the building mesh - cell

	FString baseName = "MeshCell_";
	baseName.AppendInt(CellIndex);
	FName name(baseName);
	UStaticMeshComponent* CellMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(name);
	CellMeshComponent->SetupAttachment(RootComponent);
	// Set the mesh 
	CellMeshComponent->SetStaticMesh(CellMeshAsset.Object);
	CellMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	// Reposition
	StaticMeshComponents.Add(CellMeshComponent);
	
	CellMeshComponent->SetRelativeLocation(Translation);
	CellMeshComponent->SetRelativeRotation(Rotation);

	// Creating Cell class instances:
	FString cellInstanceBaseName = "InstanceCell_";
	cellInstanceBaseName.AppendInt(CellIndex);
	FName CellInstanceName(cellInstanceBaseName);
	UCell* Cell = CreateDefaultSubobject<UCell>(CellInstanceName);
	Cell->SetupAttachment(CellMeshComponent);
	Cell->CellSteppedEvent.AddUObject(this, &AChunk::OnCellStepped);
	Cell->CellMesh = CellMeshComponent;
	Cells.Add(Cell);

	// Creating Box Collidor Components:
	FString BoxColliderBaseName = "CollideDetector_";
	BoxColliderBaseName.AppendInt(CellIndex);
	FName BoxCollidorName(BoxColliderBaseName);
	UBoxComponent* BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(BoxCollidorName);
	BoxOverlapComponent->SetupAttachment(Cell);
	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(Cell, &UCell::OnBeginOverlap);
	BoxColliders.Add(BoxOverlapComponent);
}

void AChunk::Hide()
{
	for (UStaticMeshComponent* meshComp : StaticMeshComponents)
	{
		meshComp->SetVisibility(false); // You could also target the Cell and call the HideCell function
	}
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
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

void AChunk::OnCellStepped(UCell* SteppedCell)
{
	// Handle the event
	SteppedCell->ShowAdjecentCells(3);
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChunk::InitializeCells()
{
	for (UCell* cell : Cells)
	{
		cell->SetAdjecentCells();
	}
}
