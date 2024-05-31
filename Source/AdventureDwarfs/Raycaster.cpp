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

void URaycaster::CheckFacingObject()
{
    FVector InfrontPlayerLocation = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector StartRaycastLocation = InfrontPlayerLocation + ForwardVector * TraceDistance; // Set TraceDistance to the distance you want to trace

    FVector DownwardVector = GetOwner()->GetActorUpVector() * -1;
    FVector EndLocation = StartRaycastLocation + DownwardVector * TraceDistance;

    FHitResult HitResult;

    // Perform line trace
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartRaycastLocation, EndLocation, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(40));

    //bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, ECC_GameTraceChannel1);

    // Draw a debug line for visualization
    if (bHit)
    {
        DrawDebugSphere(GetWorld(), EndLocation,40, 16, FColor::Green, false, 1, 0, 1);
    }
    else
    {
        DrawDebugSphere(GetWorld(), EndLocation,40, 16, FColor::Red, false, 1, 0, 1);
    }

    // Check if the hit actor is the specific object you're interested in
    if (bHit && HitResult.GetActor() != nullptr)
    {
        if (HitResult.GetActor()->IsA(ACollectible::StaticClass()))
        {
        	CurrentInteractable = Cast<ACollectible>(HitResult.GetActor());
        	CurrentInteractable->StartCollect();
        	//UE_LOG(LogTemp, Log, TEXT("Collectible name= %s"), *(hit->GetName()));
        }
    }
    else
    {
	    StopUse();
    }
}

void URaycaster::StopUse()
{
	if(CurrentInteractable)
	{
		CurrentInteractable->StopCollect();
	}
}

// Called when the game starts
void URaycaster::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URaycaster::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}