// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"
#include "AdjacentCellsManager.h"
#include "Collectible.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h" // Spawning Animation needed CurveFloat and Timeline
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

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
    AdjacentManager = new AdjacentCellsManager(this);
}

// Called every frame
void UCell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    MyTimeline.TickTimeline(DeltaTime); // TODO: Check if this ticks even after the animation has finished ...
    
    if(activateRaycasting && !CellProcessed)
    {
        // TODO: Make the raycast to be every 10th frame and not every frame for example. More efficient <<<
        const FVector StartRaycastLocation = FVector(GetComponentLocation().X, GetComponentLocation().Y, GetComponentLocation().Z);
        const FVector EndLocation = StartRaycastLocation + GetOwner()->GetActorUpVector() * 300;       
        FHitResult HitResult;
        bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartRaycastLocation, EndLocation, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(40));
        if(bHit)
        {
            CellSteppedEvent.Broadcast(this); // not used
            ShowAdjacentCells(5);
            CellProcessed=true;
            //DrawDebugSphere(GetWorld(),(StartRaycastLocation + EndLocation) / 2.0f, 40.0f, 12, FColor::Green,false,1);
        }
        else
        {
            //DrawDebugSphere(GetWorld(),(StartRaycastLocation + EndLocation) / 2.0f, 40.0f, 12, FColor::Red,false,1);
        }
    }
}

void UCell::PrintLocation() const
{
    FTransform transform;    
    CellMesh->GetInstanceTransform(CellMeshIndex,transform);
	UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), transform.GetLocation().X, transform.GetLocation().Y, transform.GetLocation().Z);
}

void UCell::ShowAdjacentCells(int depth) const
{
    AdjacentManager->ShowAdjacentCells(depth);
}

void UCell::ShowCell()
{
    if(IsCellVisible == false)
    {
        IsCellVisible=true;
        CellMeshIndex = CellMesh->AddInstance(FTransform(LocalRotation,LocalLocation));
        if(SpawnedCollectible)
        {
            SpawnedCollectible->SetActorHiddenInGame(false);
        }
        // Update callback event:
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("TimelineCallback"));
                
        // Finish callback event:
        FOnTimelineEventStatic TimelineFinishedCallback;
        TimelineFinishedCallback.BindUFunction(this, FName("TimelineFinishedCallback"));
        MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);

        // Subscribe the events:
        MyTimeline.AddInterpFloat(ChunkParent->FloatCurve, TimelineCallback);
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
    float NewZ = LocalLocation.Z + Value;
    
    // Animate the Cell going up.
    FTransform NewLocation;
    CellMesh->GetInstanceTransform(CellMeshIndex, NewLocation, false);
    NewLocation.SetLocation(FVector(NewLocation.GetLocation().X, NewLocation.GetLocation().Y, NewZ));
    CellMesh->UpdateInstanceTransform(CellMeshIndex, NewLocation, false);

    // Animate the spawned collectible going up with the cell.
    if(SpawnedCollectible)
    {
        if(IsMainCollectibleParent)
        {
            FTransform SpawnableLocation;
            SpawnableLocation.SetLocation(FVector(NewLocation.GetLocation().X, NewLocation.GetLocation().Y, NewZ+170));
            SpawnedCollectible->SetActorRelativeTransform(SpawnableLocation, false);
        }
        else
        {
            SpawnedCollectible->NotifyParentsShow();
        }
    }
}

void UCell::TimelineFinishedCallback()
{
   // UE_LOG(LogTemp, Log, TEXT("FINISHED"));
}

void UCell::HideCell()
{
    CellMesh->RemoveInstance(CellMeshIndex); // Deletes the instance.
}

void UCell::Raycast(AChunk* Chunk)
{
    activateRaycasting = true;
}

void UCell::StopRaycast(AChunk* Chunk)
{
    activateRaycasting = false;
}

void UCell::SetCollectible(ACollectible* Collectible, bool IsMainParent)
{
    SpawnedCollectible = Collectible;
    IsMainCollectibleParent = IsMainParent;
    if(IsCellVisible == false)
    {
        SpawnedCollectible->SetActorHiddenInGame(true);
    }
}
