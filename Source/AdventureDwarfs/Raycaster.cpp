// Fill out your copyright notice in the Description page of Project Settings.


#include "Raycaster.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

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
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, ECC_GameTraceChannel1);

    // Draw a debug line for visualization
    if (bHit)
    {
        DrawDebugLine(GetWorld(), StartRaycastLocation, EndLocation, FColor::Green, false, 0.1f, 0, 1);
    }
    else
    {
        DrawDebugLine(GetWorld(), StartRaycastLocation, EndLocation, FColor::Red, false, 0.1f, 0, 1);
    }

    // Check if the hit actor is the specific object you're interested in
    if (bHit && HitResult.GetActor() != nullptr)
    {
        if (true)//HitResult.GetActor()->IsA(YourSpecificObjectType::StaticClass()))
        {
            UE_LOG(LogTemp, Log, TEXT("HitResult.GetActor() = %s"), *(HitResult.GetActor()->GetName()));
            // Your character is facing the specific object
            // Do something here
        }
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

