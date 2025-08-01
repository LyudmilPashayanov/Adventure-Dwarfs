// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"
#include "Collectible.h"
#include "DrawDebugHelpers.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "TweenSubsystem.h"
#include "FTweenTask.h"

// Sets default values for this component's properties
UCell::UCell()
{
}

void UCell::PrintLocation() const
{
    FTransform transform;    
    CellMesh->GetInstanceTransform(CellMeshIndex,transform);
	UE_LOG(LogTemp, Log, TEXT("current position is: x- %f,y- %f,z- %f"), transform.GetLocation().X, transform.GetLocation().Y, transform.GetLocation().Z);
}

void UCell::ShowCell()
{
    if (IsCellVisible == false)
    {
        IsCellVisible = true;

        CellMeshIndex = CellMesh->AddInstance(FTransform(LocalRotation, OriginCenterLocation, CellScale));

        if (SpawnedCollectible)
        {
            SpawnedCollectible->SetActorHiddenInGame(false);
        }

       if (UWorld* World = GetWorld())
        {
            UTweenSubsystem* TweenSubsystem = World->GetSubsystem<UTweenSubsystem>();
            if (TweenSubsystem && ChunkParent && ChunkParent->FloatCurve)
            {
                FTweenTask Tween;
                Tween.Duration = 1.0f;
                Tween.Curve = ChunkParent->FloatCurve; // Curve is from -250 to 0.

                FVector CellStartLocation = OriginCenterLocation;
                FVector SpawnableStartLocation;
                if (SpawnedCollectible)
                {
                    if (IsMainCollectibleParent)
                    {
                        SpawnableStartLocation = SpawnedCollectible->GetActorTransform().GetLocation();
                    }
                }
                float MoveDistance = 3.0f;

                Tween.OnUpdate = [this, CellStartLocation, MoveDistance, SpawnableStartLocation](float Value)
                {
                    FTransform InstanceTransform;
                    if (CellMesh->GetInstanceTransform(CellMeshIndex, InstanceTransform, false))
                    {
                        InstanceTransform.SetLocation({CellStartLocation.X, CellStartLocation.Y, CellStartLocation.Z + Value * MoveDistance});

                        CellMesh->UpdateInstanceTransform(CellMeshIndex, InstanceTransform, false);

                        if (SpawnedCollectible)
                        {
                            if (IsMainCollectibleParent)
                            {
                                FTransform SpawnableTransform;

                                SpawnableTransform.SetLocation({SpawnableStartLocation.X, SpawnableStartLocation.Y, SpawnableStartLocation.Z + Value * MoveDistance});
                                SpawnedCollectible->SetActorRelativeTransform(SpawnableTransform, false);
                            }
                            else
                            {
                                SpawnedCollectible->NotifyParentsShow();
                            }
                        }
                    }
                };

                Tween.OnComplete = [this]()
                {
                    /*FTransform InstanceTransform;
                    if (CellMesh->GetInstanceTransform(CellMeshIndex, InstanceTransform, false))
                    {
                        FVector NewLocation = InstanceTransform.GetLocation();
                        UE_LOG(LogTemp, Log, TEXT("Cell location in memory = %f, %f, %f AND Cell instance location = %f, %f, %f"), LocalLocation.X, LocalLocation.Y, LocalLocation.Z, NewLocation.X, NewLocation.Y, NewLocation.Z);
                    }*/
                };

                TweenSubsystem->AddTween(MoveTemp(Tween));
            }
        }
    }
}

void UCell::HideCell()
{
    CellMesh->RemoveInstance(CellMeshIndex); // Deletes the instance.
    IsCellVisible = false;
}

void UCell::SetLocation(const FVector& Location)
{
    OriginCenterLocation = Location;
    // Get mesh bounds (use original unscaled bounds)
    UStaticMesh* mesh = CellMesh->GetStaticMesh();
    if (mesh)
    {
        FVector BoxExtent = mesh->GetBoundingBox().GetExtent();
        float TopSurfaceZ = OriginCenterLocation.Z + BoxExtent.Z * CellScale.Z;
        CellSurface = FVector(Location.X, Location.Y, TopSurfaceZ);
    }
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

void UCell::InitTransform(const FVector& Location, const FRotator& Rotation, const FVector& Scale)
{
    LocalRotation = Rotation;
    CellScale = Scale;
    SetLocation(Location);
    Height = FMath::RoundToInt(CellSurface.Z / 50);
}

bool UCell::IsWalkable()
{
    if (SpawnedCollectible)
    {
        return false;
    }

    return true;
}

void UCell::Highlight(int number)
{
    DrawDebugString(GetWorld(), CellSurface + FVector(0, 0, 30), FString::Printf(TEXT("%d"), number), nullptr, FColor::Red, -1,false, 30);
}
