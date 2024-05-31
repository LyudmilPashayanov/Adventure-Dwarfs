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

void ACollectible::StartCollect()
{
	if (bIsBeingCollected == false)
	{
		HoldStartTime = GetWorld()->GetTimeSeconds();
		bIsBeingCollected = true;
		GetWorldTimerManager().SetTimer(HoldTimerHandle, this, &ACollectible::CheckHoldDuration, 0.1f, true);
	}
}

void ACollectible::CheckHoldDuration()
{
	if (bIsBeingCollected)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Log, TEXT("Collecting time: %f" ),CurrentTime);
		if (CurrentTime - HoldStartTime >= HoldDurationThreshold)
		{
			UE_LOG(LogTemp, Log, TEXT("Held for enough time"));
			// Perform desired action here
			bIsBeingCollected = false;
			GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
		}
	}
}

void ACollectible::StopCollect()
{
	bIsBeingCollected = false;
	GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
}


