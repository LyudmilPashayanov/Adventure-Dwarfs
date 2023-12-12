// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "AdjecentDirections.h"
#include "Cell.h"
#include "Templates/SharedPointer.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;
	// SPAWN MESHES:
	int posX = 0;
	int posY = 0;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CellMeshAsset(TEXT("StaticMesh'/Game/LyudmilContent/Chunks/pCube24.pCube24'"));
	for (int i = 0; i < 10; i++)
	{
		for (int k = 0; k < 10; k++)
		{
			posX = (105 * i);
			posY = (105 * k);

			// Creating Cell mesh components:
			FString baseName = "MeshCell_";
			baseName.AppendInt(i);
			baseName.AppendInt(k);
			FName name(baseName);
			UStaticMeshComponent* CellMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(name);
			CellMeshComponent->SetupAttachment(RootComponent);
			// Set the mesh 
			CellMeshComponent->SetStaticMesh(CellMeshAsset.Object);
			// Reposition
			StaticMeshComponents.Add(CellMeshComponent);
			CellMeshComponent->SetRelativeLocation(FVector(posX-470, posY-470, 0.0)); // -470, so that the center of the grid is the center of the blueprint actor.
			CellMeshComponent->SetRelativeRotation(FRotator(90, 0.0, 0.0));
			
			
			// Creating Cell class instances:
			FString cellInstanceBaseName = "InstanceCell_";
			cellInstanceBaseName.AppendInt(i);
			cellInstanceBaseName.AppendInt(k);
			FName CellInstanceName(cellInstanceBaseName);
			UCell* Cell = CreateDefaultSubobject<UCell>(CellInstanceName);
			Cell->SetupAttachment(CellMeshComponent);
			Cell->CellSteppedEvent.AddUObject(this, &AChunk::OnCellStepped);
			Cell->CellMesh = CellMeshComponent;
			Cells.Add(Cell);
			
			
			// Creating Box Collidor Components:
			FString BoxColliderBaseName = "CollideDetector_";
			BoxColliderBaseName.AppendInt(i);
			BoxColliderBaseName.AppendInt(k);
			FName BoxCollidorName(BoxColliderBaseName);
			UBoxComponent* BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(BoxCollidorName);
			BoxOverlapComponent->SetupAttachment(Cell);
			BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(Cell, &UCell::OnBeginOverlap);
			BoxColliders.Add(BoxOverlapComponent);
		}
	}
}

void AChunk::LoadJsonData(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		const TArray<TSharedPtr<FJsonValue>>* CellsData;
		if (JsonObject->TryGetArrayField("CellsData", CellsData))
		{
			for (const auto& CellData : *CellsData)
			{
				TSharedPtr<FJsonObject> CellObject = CellData->AsObject();

				const TArray<TSharedPtr<FJsonValue>>* TranslationArray;
				FVector Translation;
				FRotator Rotation;
				FVector Scale;

				if(CellObject->TryGetArrayField("translation", TranslationArray))
				{
					Translation.X = (*TranslationArray)[0]->AsNumber();
					Translation.Y = (*TranslationArray)[1]->AsNumber();
					Translation.Z = (*TranslationArray)[2]->AsNumber();
				}

				UE_LOG(LogTemp, Log, TEXT("Vector information = x- %f, y- %f, z-%f"),Translation.X, Translation.Y, Translation.Z);
			}
		}
	}
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
	LoadJsonData(ChunkData);
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

