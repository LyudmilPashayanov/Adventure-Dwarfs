// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"
#include "DrawDebugHelpers.h"
#include "AdjecantDirections.h"
#include "Curves/CurveFloat.h" // Spawning Animation needed CurveFloat and Timeline
#include "AdjecantManager.h"
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
    //UE_LOG(LogTemp, Log, TEXT("CellMesh->Bounds.BoxExtent.X*2 %f"), CellMesh->GetStaticMesh()->GetBounds().BoxExtent.X*2);

    Adjecants = new AdjecantManager<UCell>(CellMesh->GetStaticMesh()->GetBounds().BoxExtent.X*2, CellMesh->GetComponentLocation());
}

// Called every frame
void UCell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    MyTimeline.TickTimeline(DeltaTime); // TODO: Check if this ticks even after the animation has finished ...
    if(activateRaycasting)
    {
        // TODO: Make the raycast to be every 10th frame and not every frame for example. More efficient <<<
        FVector StartRaycastLocation = FVector(OriginalLocation.X, OriginalLocation.Y, OriginalLocation.Z);
        FVector EndLocation = StartRaycastLocation + GetOwner()->GetActorUpVector() * 300;       
        FHitResult HitResult;

        bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartRaycastLocation, EndLocation,
            FQuat::Identity,
            ECC_GameTraceChannel2,
            FCollisionShape::MakeSphere(40));
        
        if(bHit)
        {
            CellSteppedEvent.Broadcast(this);
            DrawDebugSphere(GetWorld(),(StartRaycastLocation + EndLocation) / 2.0f, 40.0f, 12, FColor::Green);
        }
        else
        {
            DrawDebugSphere(GetWorld(),(StartRaycastLocation + EndLocation) / 2.0f, 40.0f, 12, FColor::Red);
        }
    }
}

void UCell::PrintLocation()
{
    FTransform transform;    
    CellMesh->GetInstanceTransform(CellMeshIndex,transform);
	UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), transform.GetLocation().X, transform.GetLocation().Y, transform.GetLocation().Z);
}

void UCell::SetAdjacentCells()
{
    FVector upVector = GetOwner()->GetActorUpVector();
    PrintLocation();
    Adjecants->SetAdjacentObjects(upVector, GetWorld());
}

void UCell::ShowAdjacentCells(int depth, UCurveFloat* floatCurve)
{
    CellMeshIndex = CellMesh->AddInstance(FTransform(OriginalRotation, OriginalLocation));

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
    if(IsCellVisible == false)
    {
        IsCellVisible=true;
        CellMeshIndex = CellMesh->AddInstance(FTransform(OriginalRotation,OriginalLocation));

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
    float NewZ = OriginalLocation.Z + Value;
    FTransform NewLocation;
    CellMesh->GetInstanceTransform(CellMeshIndex,NewLocation);
    NewLocation.GetLocation().Set(NewLocation.GetLocation().X, NewLocation.GetLocation().Y, NewZ);
    CellMesh->UpdateInstanceTransform(CellMeshIndex, NewLocation, false); // TODO: If it doesn't work try set it to FALSE
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
    UE_LOG(LogTemp, Log, TEXT(" Raycast Raycast Raycast Raycast Raycast"));
    activateRaycasting=true;
}

void UCell::StopRaycast(AChunk* Chunk)
{
    activateRaycasting=false;
}
