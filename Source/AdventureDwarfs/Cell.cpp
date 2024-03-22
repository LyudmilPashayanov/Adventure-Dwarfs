// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"
#include "DrawDebugHelpers.h"
#include "AdjecantDirections.h"

// Spawning Animation needed CurveFloat and Timeline
#include "Curves/CurveFloat.h"
#include "AdjecantManager.h"
#include "AdventureDwarfsCharacter.h"

#include <Components/BoxComponent.h>

// Sets default values for this component's properties
UCell::UCell()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCell::BeginPlay()
{
	Super::BeginPlay();
    originalLocation = CellMesh->GetRelativeLocation();
    Adjecants = new AdjecantManager<UCell>(CellMesh->Bounds.BoxExtent.X*2, CellMesh->GetComponentLocation());
}

// Called every frame
void UCell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    MyTimeline.TickTimeline(DeltaTime);
}

void UCell::PrintLocation()
{
	UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), CellMesh->GetComponentLocation().X, CellMesh->GetComponentLocation().Y, CellMesh->GetComponentLocation().Z);
	UE_LOG(LogTemp, Log, TEXT("current size is: x- %f,y- %f,z- %f"), CellMesh->Bounds.BoxExtent.X*2, CellMesh->Bounds.BoxExtent.Y*2, CellMesh->Bounds.BoxExtent.Z*2);
}

void UCell::SetAdjacentCells()
{
    FVector upVector = GetOwner()->GetActorUpVector();
    PrintLocation();
    Adjecants->SetAdjacentObjects(upVector, GetWorld());
}

void UCell::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(AAdventureDwarfsCharacter::StaticClass()))
    {
        CellSteppedEvent.Broadcast(this); // TODO: Invokation is twice, because 2 colliders are registered for the Player. Make it one <<<
    }
}

void UCell::ShowAdjacentCells(int depth, UCurveFloat* floatCurve)
{
    depth--;
    for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
    {
        AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
        UCell* CellToCheck = Adjecants->GetAdjacentObject(currentEnumValue);
        if (CellToCheck) 
        {
            CellToCheck->ShowCell(floatCurve);
            if (depth > 0)
            {
                CellToCheck->ShowAdjacentCells(depth, floatCurve);
            }
        }
    }
}

void UCell::ShowCell(UCurveFloat* floatCurve)
{
    if (CellMesh->IsVisible() == false) {
        // UE_LOG(LogTemp, Log, TEXT("SHOW CELL %s"),*GetOwner()->GetName());
        CellMesh->SetVisibility(true);

        // Update callback event:
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("TimelineCallback"));
                
        // Finish callback event:
        FOnTimelineEventStatic TimelineFinishedCallback;
        TimelineFinishedCallback.BindUFunction(this, FName("TimelineFinishedCallback"));
        MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);

        // Subscribe the events:
        MyTimeline.AddInterpFloat(floatCurve, TimelineCallback);
        // Set the timeline's properties (e.g., length, loop, etc.)
        MyTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
        MyTimeline.SetTimelineLength(1.0f); // 1 second
        MyTimeline.SetLooping(false);
        // Play the timeline
        MyTimeline.PlayFromStart();
    }
}

void UCell::TimelineCallback(float Value)
{
    // Interpolate the value using the FloatCurve
    //UE_LOG(LogTemp, Log, TEXT("is it working? %s with value: %f"), *GetName(),Value);
    //UE_LOG(LogTemp, Log, TEXT("originalLocation : %s"),*originalLocation.ToString());
    float NewZ = originalLocation.Z + Value;
    // Set the new location
    FVector NewLocation = CellMesh->GetRelativeLocation();
    NewLocation.Z = NewZ;
    CellMesh->SetRelativeLocation(NewLocation);
}

void UCell::TimelineFinishedCallback()
{
   // UE_LOG(LogTemp, Log, TEXT("FINISHED"));
}

void UCell::HideCell()
{
    CellMesh->SetVisibility(false);
}











//void UCell::RaycastForPlayer()
//{
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
//}

