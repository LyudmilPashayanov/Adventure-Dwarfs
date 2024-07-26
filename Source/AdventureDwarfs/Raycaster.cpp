// Fill out your copyright notice in the Description page of Project Settings.


#include "Raycaster.h"

#include "Collectible.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
URaycaster::URaycaster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URaycaster::BeginPlay()
{
	Super::BeginPlay();
}

void URaycaster::CheckFacingObject()
{
    FVector InfrontPlayerLocation = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector StartRaycastLocation = InfrontPlayerLocation + ForwardVector * TraceDistance; // Set TraceDistance to the distance you want to trace

    FVector DownwardVector = GetOwner()->GetActorUpVector() * -1;
    FVector EndLocation = StartRaycastLocation + DownwardVector * TraceDistance;

    FHitResult HitResult;

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartRaycastLocation, EndLocation, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(40));

    //bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, ECC_GameTraceChannel1);
	
    if (bHit)
    {
        DrawDebugSphere(GetWorld(), EndLocation,40, 16, FColor::Green, false, 1, 0, 1);
    }
    else
    {
        DrawDebugSphere(GetWorld(), EndLocation,40, 16, FColor::Red, false, 1, 0, 1);
    }

    if (bHit && HitResult.GetActor() != nullptr)
    {
        if (HitResult.GetActor()->IsA(ACollectible::StaticClass()))
        {
	        if (bIsBeingCollected == false)
	        {
	        	CurrentInteractable = Cast<ACollectible>(HitResult.GetActor());
	        	HoldStartTime = GetWorld()->GetTimeSeconds();
	        	bIsBeingCollected = true;
	        	GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &URaycaster::CollectingUpdate, 0.03f, true);
	        }
        }
    }
    else
    {
	    StopUse();
    }
}

void URaycaster::StopUse()
{
	bIsBeingCollected = false;
	GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
	OnCollectionStopped.Broadcast();
}

void URaycaster::CollectingUpdate()
{
	if (bIsBeingCollected && CurrentInteractable)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float HoldTime = CurrentTime - HoldStartTime;
		float requiredTime = CurrentInteractable->GetCollectTimeRequired();
		float ClampedValue = FMath::Clamp(HoldTime, 0, requiredTime);
		float NormalizedValue = (ClampedValue - 0) / (requiredTime - 0);
		OnCollectionStarted.Broadcast(NormalizedValue);
		
		if (HoldTime >= CurrentInteractable->GetCollectTimeRequired())
		{
			UE_LOG(LogTemp, Log, TEXT("Held for enough time"));
			bIsBeingCollected = false;
			GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
			CurrentInteractable->Collect();
		}
	}
}