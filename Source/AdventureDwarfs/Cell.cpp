// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include "AdventureDwarfsCharacter.h"
#include "Raycaster.h"
#include "AdjecentDirections.h"
#include "GridPosition.h"
#include <optional>
#include <Components/BoxComponent.h>

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxComponent;
	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxOverlapDetector"));
	BoxOverlapComponent->SetupAttachment(RootComponent);
	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ACell::OnBeginOverlap);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}
// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
}

void ACell::Init(GridPosition position)
{
	pos = position;
}

void ACell::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->IsA(AAdventureDwarfsCharacter::StaticClass()))
	{
		CellSteppedEvent.Broadcast(this);
	}
}

bool ACell::CheckAdjecentCell(AdjecentDirections directionToCheck)
{
	FHitResult hit;
	ACell* CellToCheck = GetAdjecentCell(directionToCheck);
	GridPosition positionToCheck = GetAdjecentPosition(directionToCheck);

	if (CellToCheck==nullptr) // do the raycast only if there is not adjecent
	{
		if (RaycastChecker->RaycastDownToPosition(positionToCheck.X, positionToCheck.Y, hit)) // check top left
		{
			if (hit.GetActor()->IsA(ACell::StaticClass()))
			{
				ACell* cellAtPos = Cast<ACell>(hit.GetActor());
				//CellToCheck = cellAtPos; //TODO: Why can't I do this, but I have to do the bottom one?
				SetAdjecentCell(cellAtPos, directionToCheck);
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

void ACell::SetAdjecentCell(ACell* cell, AdjecentDirections directionToGet)
{
	switch (directionToGet)
	{
	case AdjecentDirections::TopLeft:
		Adjecent_TL = cell;
		break;
	case AdjecentDirections::TopCenter:
		Adjecent_TC = cell;
		break;
	case AdjecentDirections::TopRight:
		Adjecent_TR = cell;
		break;
	case AdjecentDirections::Left:
		Adjecent_L = cell;
		break;
	case AdjecentDirections::Right:
		Adjecent_R = cell;
		break;
	case AdjecentDirections::BottomLeft:
		Adjecent_BL= cell;
		break;
	case AdjecentDirections::BottomCenter:
		Adjecent_BC = cell;
		break;
	case AdjecentDirections::BottomRight:
		Adjecent_BR = cell;
		break;
	}
}

ACell* ACell::GetAdjecentCell(AdjecentDirections directionToGet)
{
	switch (directionToGet)
	{
	case AdjecentDirections::TopLeft:
		return Adjecent_TL;
		break;
	case AdjecentDirections::TopCenter:
		return Adjecent_TC;
		break;
	case AdjecentDirections::TopRight:
		return Adjecent_TR;
		break;
	case AdjecentDirections::Left:
		return Adjecent_L;
		break;
	case AdjecentDirections::Right:
		return Adjecent_R;
		break;
	case AdjecentDirections::BottomLeft:
		return Adjecent_BL;
		break;
	case AdjecentDirections::BottomCenter:
		return Adjecent_BC;
		break;
	case AdjecentDirections::BottomRight:
		return Adjecent_BR;
		break;
	}
	return nullptr;
}

GridPosition ACell::GetAdjecentPosition(AdjecentDirections directionToGet)
{
	switch (directionToGet)
	{
	case AdjecentDirections::TopLeft:
		return GridPosition(pos.X + 105, pos.Y - 105);
		break;
	case AdjecentDirections::TopCenter:
		return GridPosition(pos.X + 105, pos.Y);
		break;
	case AdjecentDirections::TopRight:
		return GridPosition(pos.X + 105, pos.Y + 105);
		break;
	case AdjecentDirections::Left:
		return GridPosition(pos.X, pos.Y - 105);
		break;
	case AdjecentDirections::Right:
		return GridPosition(pos.X, pos.Y + 105);
		break;
	case AdjecentDirections::BottomLeft:
		return GridPosition(pos.X - 105, pos.Y - 105);
		break;
	case AdjecentDirections::BottomCenter:
		return GridPosition(pos.X - 105, pos.Y);
		break;
	case AdjecentDirections::BottomRight:
		return GridPosition(pos.X - 105, pos.Y + 105);
		break;
	}
	return GridPosition(0,0);
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACell::InjectRaycaster(URaycaster* raycastClass)
{
	RaycastChecker = raycastClass;
}

