// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Cell.h"
#include <Components/BoxComponent.h>


// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	for (UStaticMeshComponent* MeshComponent : StaticMeshComponents)
	{
		UCell* CellInstance = NewObject<UCell>(this);
		CellInstance->RegisterComponent();
		CellInstance->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		if (CellInstance)
		{
			cells.Add(CellInstance);
			CellInstance->PrintLocation();
		}
	}
}

void AChunk::OnCellEventReceived(UCell* SteppedCell)
{
	// Handle the event
	SteppedCell->PrintLocation();
	/*for (int i = 0; i < static_cast<int>(AdjecentDirections::Count); ++i) {
		AdjecentDirections currentEnumValue = static_cast<AdjecentDirections>(i);

		if (!SteppedCell->CheckAdjecentCell(currentEnumValue))
		{
			GridPosition pos = SteppedCell->GetAdjecentPosition(currentEnumValue);
			SpawnCell(pos.X, pos.Y);
		}
	}*/
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

