// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"

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
	Size = data->Size;
	Orientation = data->Orientation;
	SetActorScale3D(FVector(0.05f,0.05f,0.05f));
}


