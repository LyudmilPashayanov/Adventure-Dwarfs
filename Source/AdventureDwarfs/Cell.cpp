// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include "DrawDebugHelpers.h"
#include "AdventureDwarfsCharacter.h"
#include <Components/BoxComponent.h>

// Sets default values for this component's properties
UCell::UCell()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    UE_LOG(LogTemp, Log, TEXT("constructor"));
}

// Called when the game starts
void UCell::BeginPlay()
{
	Super::BeginPlay();
    ShouldRaycast = true;
}

void UCell::PrintLocation()
{
	UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), GetComponentLocation().X, GetComponentLocation().Y, GetComponentLocation().Z);
}

// Called every frame
void UCell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    RaycastForPlayer();
}

void UCell::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(AAdventureDwarfsCharacter::StaticClass()))
    {
        UE_LOG(LogTemp, Log, TEXT("KRESHTQQQ KRESHTQQQ KRESHTQQQ KRESHTQQQ KRESHTQQQ KRESHTQQQ KRESHTQQQ KRESHTQQQ KRESHTQQQ KRESHTQQQ "));
        CellSteppedEvent.Broadcast(this);
    }
}

void UCell::RaycastForPlayer()
{
    //FVector StartRaycastLocation = GetComponentLocation(); 
    //FVector EndLocation = StartRaycastLocation + GetRightVector() * -1 * TraceDistance; // Using the "right" vector, because the Cells are turned 90 degrees in the scene.
    //if (ShouldRaycast)
    //{
    //    FHitResult HitResult;
    //    // Single line raycasted:
    //    //bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, ECC_GameTraceChannel2);
    //    
    //    // Capsule Shape raycasted:
    //    FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(20, /* Half Height */ 1);
    //    FCollisionQueryParams CollisionParams;
    //    CollisionParams.AddIgnoredActor(GetOwner()); // Ignore the actor initiating the trace
    //    bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartRaycastLocation, EndLocation, FQuat::Identity, ECC_GameTraceChannel2, CapsuleShape, CollisionParams);
    //    if (bHit)
    //    {
    //        //DrawDebugCapsule(GetWorld(), (StartRaycastLocation + EndLocation) * 0.5f,0.5f, 20, FQuat::Identity, FColor::Green, false, 0.1f, 0, 1.0f);
    //        DrawDebugLine(GetWorld(), StartRaycastLocation, EndLocation, FColor::Green, false, 0.1f, 0, 20);
    //        if (HitResult.GetActor() && HitResult.GetActor()->IsA(AAdventureDwarfsCharacter::StaticClass()))
    //        {
    //            CellSteppedEvent.Broadcast(this);
    //            //UE_LOG(LogTemp, Log, TEXT("PLAYER IS ON ME !!!!!!!!!!!!!!!!!!! "));
    //        }
    //    }
    //    else
    //    {
    //        //DrawDebugCapsule(GetWorld(), (StartRaycastLocation + EndLocation) * 0.5f, 0.5f, 20, FQuat::Identity, FColor::Yellow, false, 2.0f, 0, 1.0f);
    //        DrawDebugLine(GetWorld(), StartRaycastLocation, EndLocation, FColor::Yellow, false, 0.1f, 0, 1);
    //    }
    //}
}

