// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"
#include "DrawDebugHelpers.h"
#include "AdjecentDirections.h"
#include "GridPosition.h"
#include "Raycaster.h"

// Spawning Animation needed CurveFloat and Timeline
#include "Curves/CurveFloat.h"

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
}

// Called every frame
void UCell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    MyTimeline.TickTimeline(DeltaTime);
}

void UCell::PrintLocation()
{
	UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), GetComponentLocation().X, GetComponentLocation().Y, GetComponentLocation().Z);
}

void UCell::SetAdjecentCells()
{
    FHitResult hit;
    for (int i = 0; i < static_cast<int>(AdjecentDirections::Count); ++i) 
    {
        AdjecentDirections currentEnumValue = static_cast<AdjecentDirections>(i);
        UCell* CellToCheck = GetAdjecentCell(currentEnumValue);
        GridPosition positionToCheck = GetAdjecentPosition(currentEnumValue);
        if (RaycastAdjecentCells(positionToCheck.X, positionToCheck.Y, hit))
        {
            USceneComponent* CellSceneComponent = hit.GetComponent()->GetChildComponent(0);
            if (CellSceneComponent->IsA(UCell::StaticClass())) // maybe not very flexible :P 
            {
                UE_LOG(LogTemp, Log, TEXT("hit CHILD INDEX 0 name: %s"), *CellSceneComponent->GetName());
                UCell* cellAtPos = Cast<UCell>(CellSceneComponent);
                switch (currentEnumValue)
                {
                case AdjecentDirections::TopLeft:
                    Adjecent_TL = cellAtPos;
                    break;
                case AdjecentDirections::TopCenter:
                    Adjecent_TC = cellAtPos;
                    break;
                case AdjecentDirections::TopRight:
                    Adjecent_TR = cellAtPos;
                    break;
                case AdjecentDirections::Left:
                    Adjecent_L = cellAtPos;
                    break;
                case AdjecentDirections::Right:
                    Adjecent_R = cellAtPos;
                    break;
                case AdjecentDirections::BottomLeft:
                    Adjecent_BL = cellAtPos;
                    break;
                case AdjecentDirections::BottomCenter:
                    Adjecent_BC = cellAtPos;
                    break;
                case AdjecentDirections::BottomRight:
                    Adjecent_BR = cellAtPos;
                    break;
                }
            }
        }
    }
}

void UCell::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(AAdventureDwarfsCharacter::StaticClass()))
    {
        CellSteppedEvent.Broadcast(this); // TODO: Invokation is twice, because 2 colliders are registered for the Player. Make it one <<<
    }
}

bool UCell::RaycastAdjecentCells(int posX, int posY, FHitResult& result)
{
    FVector StartRaycastLocation = FVector(posX, posY, 1000);
    FVector DownwardVector = GetOwner()->GetActorUpVector() * -1;
    FVector EndLocation = StartRaycastLocation + DownwardVector * TraceDistance;

    FHitResult HitResult;

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, ECC_GameTraceChannel1);

    if (bHit)
    {
        result = HitResult;
        //DrawDebugLine(GetWorld(), StartRaycastLocation, EndLocation, FColor::Green, false, 3, 0, 1);
    }
    else
    {
        //DrawDebugLine(GetWorld(), StartRaycastLocation, EndLocation, FColor::Red, false, 3, 0, 1);
    }
    return bHit;
}

UCell* UCell::GetAdjecentCell(AdjecentDirections directionToGet)
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

GridPosition UCell::GetAdjecentPosition(AdjecentDirections directionToGet)
{
    switch (directionToGet)
    {
    case AdjecentDirections::TopLeft:
        return GridPosition(GetComponentLocation().X + 105, GetComponentLocation().Y - 105);
        break;
    case AdjecentDirections::TopCenter:
        return GridPosition(GetComponentLocation().X + 105, GetComponentLocation().Y);
        break;
    case AdjecentDirections::TopRight:
        return GridPosition(GetComponentLocation().X + 105, GetComponentLocation().Y + 105);
        break;
    case AdjecentDirections::Left:
        return GridPosition(GetComponentLocation().X, GetComponentLocation().Y - 105);
        break;
    case AdjecentDirections::Right:
        return GridPosition(GetComponentLocation().X, GetComponentLocation().Y + 105);
        break;
    case AdjecentDirections::BottomLeft:
        return GridPosition(GetComponentLocation().X - 105, GetComponentLocation().Y - 105);
        break;
    case AdjecentDirections::BottomCenter:
        return GridPosition(GetComponentLocation().X - 105, GetComponentLocation().Y);
        break;
    case AdjecentDirections::BottomRight:
        return GridPosition(GetComponentLocation().X - 105, GetComponentLocation().Y + 105);
        break;
    }
    return GridPosition(0, 0);
}

void UCell::ShowAdjecentCells(int depth, UCurveFloat* floatCurve)
{
    depth--;
    for (int i = 0; i < static_cast<int>(AdjecentDirections::Count); ++i)
    {
        AdjecentDirections currentEnumValue = static_cast<AdjecentDirections>(i);
        UCell* CellToCheck = GetAdjecentCell(currentEnumValue);
        //UE_LOG(LogTemp, Log, TEXT("trying to show adjecent cells"));
        if (CellToCheck) 
        {
            //UE_LOG(LogTemp, Log, TEXT("showing adjecent cells"));
            CellToCheck->ShowCell(floatCurve);
            if (depth > 0)
            {
                CellToCheck->ShowAdjecentCells(depth, floatCurve);
            }
        }
    }
}

void UCell::ShowCell(UCurveFloat* floatCurve)
{
    if (CellMesh->IsVisible() == false) {
        UE_LOG(LogTemp, Log, TEXT("SHOW CELL %s"),*GetOwner()->GetName());
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
    if (finished == false)
    {
        // Interpolate the value using the FloatCurve
        UE_LOG(LogTemp, Log, TEXT("is it working? %s with value: %f"), *GetName(),Value);
        //UE_LOG(LogTemp, Log, TEXT("originalLocation : %s"),*originalLocation.ToString());
        float NewZ = originalLocation.Z + Value;
        // Set the new location
        FVector NewLocation = CellMesh->GetRelativeLocation();
        NewLocation.Z = NewZ;
        CellMesh->SetRelativeLocation(NewLocation);
    }
}

void UCell::TimelineFinishedCallback()
{
    //finished = true;
    UE_LOG(LogTemp, Log, TEXT("FINISHED"));
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

