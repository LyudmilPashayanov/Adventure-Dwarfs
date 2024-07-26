// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"

#include "Cell.h"
#include "CollectibleDataAsset.h"

// Sets default values
ACollectible::ACollectible()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
}

void ACollectible::Init(UCollectibleDataAsset* data)
{
	StaticMeshComponent->SetStaticMesh(data->Mesh);
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Size = data->Size;
	Orientation = data->Orientation;
}

void ACollectible::NotifyParentsShow()
{
	for (auto parentCell : ParentCells)
	{
		parentCell->ShowCell();
	}
}

void ACollectible::Collect()
{
	UE_LOG(LogTemp, Log, TEXT("Collected"));
	GetWorld()->DestroyActor(this);
}

float ACollectible::GetCollectTimeRequired() const
{
	return collectTimeRequired;
}


