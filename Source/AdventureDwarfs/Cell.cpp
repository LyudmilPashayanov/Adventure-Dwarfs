// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"
#include "AdjacentCellsManager.h"
#include "Collectible.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h" // Spawning Animation needed CurveFloat and Timeline
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "TweenSubsystem.h"
#include "FTweenTask.h"

// Sets default values for this component's properties
UCell::UCell()
{
    AdjacentManager = new AdjacentCellsManager(this);
}

void UCell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if(activateRaycasting && !CellProcessed)
    {
       
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
    if (IsCellVisible == false)
    {
        IsCellVisible = true;

        CellMeshIndex = CellMesh->AddInstance(FTransform(LocalRotation, LocalLocation, CellScale));

        if (SpawnedCollectible)
        {
            SpawnedCollectible->SetActorHiddenInGame(false);
        }

        if (UWorld* World = GetWorld())
        {
            UTweenSubsystem* TweenSubsystem = World->GetSubsystem<UTweenSubsystem>();
            if (TweenSubsystem && ChunkParent && ChunkParent->FloatCurve)
            {
			    //UE_LOG(LogTemp, Log, TEXT("should animate"));
                FTweenTask Tween;
                Tween.Duration = 1.0f;
                Tween.Curve = ChunkParent->FloatCurve;

                FVector StartLocation = LocalLocation;
                float MoveDistance = 3.0f;

                Tween.OnUpdate = [this, StartLocation, MoveDistance](float Value)
                {
                    FTransform InstanceTransform;
                    if (CellMesh->GetInstanceTransform(CellMeshIndex, InstanceTransform, false))
                    {
                        FVector NewLocation = InstanceTransform.GetLocation();
                        NewLocation.Z = StartLocation.Z + Value * MoveDistance;
                        InstanceTransform.SetLocation(NewLocation);

                        CellMesh->UpdateInstanceTransform(CellMeshIndex, InstanceTransform, false);

                        if (SpawnedCollectible)
                        {
                            if (IsMainCollectibleParent)
                            {
                                FTransform SpawnableLocation;
                                SpawnableLocation.SetLocation(FVector(NewLocation.X, NewLocation.Y, NewLocation.Z + 170));
                                SpawnedCollectible->SetActorRelativeTransform(SpawnableLocation, false);
                            }
                            else
                            {
                                SpawnedCollectible->NotifyParentsShow();
                            }
                        }
                    }
                };

                Tween.OnComplete = []()
                {
                    //UE_LOG(LogTemp, Log, TEXT("Cell animation finished"));
                };

                TweenSubsystem->AddTween(MoveTemp(Tween));
            }
        }
    }
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
