// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "AdjecantDirections.h"
#include "Cell.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Templates/SharedPointer.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

// Spawning Animation needed CurveFloat and Timeline
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "AdjecantManager.h"

#include "Engine/DataTable.h"
#include "ChunkDataField.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 
	
	Adjecants = new AdjecantManager<AChunk>();
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	ConstructorHelpers::FObjectFinder<UDataTable> JsonConstructData = GetGridConstructJsonPath();
	if (JsonConstructData.Succeeded())
	{
		//UE_LOG(LogTemp, Log, TEXT("FILE EXISTS: %s"), *toUse.Object->GetName());
	
		TArray<FChunkDataField*> CellsData;
		JsonConstructData.Object->GetAllRows<FChunkDataField>("", CellsData);
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
	 	UE_LOG(LogTemp, Log, TEXT("Failed to fetch JSON data table needed for constructing the chunk."));
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
	///* TODO: Remove this in the future, when you are using normal cell */ CellMeshComponent->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
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
	//BoxOverlapComponent->SetBoxExtent(FVector(32.0f, 32.0f, 150.0f)); // TODO: Not working for the flat chunk (only for hill chunk), try a way to fix it OR just change the scale of the object.
	//BoxOverlapComponent->UpdateBodySetup();
	BoxOverlapComponent->SetWorldLocation(FVector(0, 0, 150));
	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(Cell, &UCell::OnBeginOverlap);
	///* TODO: Remove this in the future, when you are using normal cell */ BoxOverlapComponent->SetWorldScale3D(FVector(10, 10, 10));
	BoxColliders.Add(BoxOverlapComponent);
}

void AChunk::Hide()
{
	for (UStaticMeshComponent* meshComp : StaticMeshComponents)
	{
		meshComp->SetVisibility(false); // TODO: -> BETTER --- You could also target the Cell and call the HideCell function
	}
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	Adjecants->SetAdjecantObjects(GetActorUpVector(), GetWorld(), GetActorLocation());
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
	SteppedCell->ShowAdjecentCells(4, FloatCurve);
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

