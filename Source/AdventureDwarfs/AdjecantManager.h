// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AdjecantManager.h"
#include "AdjecantDirections.h"
#include "GridPosition.h"
#include "Raycaster.h"
#include "CoreMinimal.h"


enum class AdjecantDirections;
class GridPosition;

template<class T>
class ADVENTUREDWARFS_API AdjecantManager
{
public:
    
    UPROPERTY()
    USceneComponent* Parent;

    inline AdjecantManager(USceneComponent* ParentObject)
    {
        Parent = ParentObject;
    }
	UPROPERTY()
	T* Adjecent_TL = nullptr; // top left adjecent
	UPROPERTY()
	T* Adjecent_TC = nullptr; // top center adjecent
	UPROPERTY()
	T* Adjecent_TR = nullptr; // top right adjecent 
	UPROPERTY()
	T* Adjecent_L = nullptr;  // left adjecent
	UPROPERTY()
	T* Adjecent_R = nullptr;  // right adjecent 
	UPROPERTY()
	T* Adjecent_BL = nullptr; // bottom left adjecent
	UPROPERTY()
	T* Adjecent_BC = nullptr; // bottom center adjecent
	UPROPERTY()
	T* Adjecent_BR = nullptr; // bottom right adjecent 

	void SetAdjecantObjects() 
	{
        FHitResult hit;
        for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
        {
            AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
            GridPosition positionToCheck = GetAdjecentPosition(currentEnumValue);
            if (RaycastAdjecentObjects(positionToCheck.X, positionToCheck.Y, hit))
            {
                USceneComponent* ObjectSceneComponent = hit.GetComponent()->GetChildComponent(0);
                if (ObjectSceneComponent->IsA(T::StaticClass())) // maybe not very flexible :P 
                {
                    // UE_LOG(LogTemp, Log, TEXT("hit CHILD INDEX 0 name: %s"), *CellSceneComponent->GetName());
                    T* objectAtPos = Cast<T>(ObjectSceneComponent);
                    switch (currentEnumValue)
                    {
                    case AdjecantDirections::TopLeft:
                        Adjecent_TL = objectAtPos;
                        break;
                    case AdjecantDirections::TopCenter:
                        Adjecent_TC = objectAtPos;
                        break;
                    case AdjecantDirections::TopRight:
                        Adjecent_TR = objectAtPos;
                        break;
                    case AdjecantDirections::Left:
                        Adjecent_L = objectAtPos;
                        break;
                    case AdjecantDirections::Right:
                        Adjecent_R = objectAtPos;
                        break;
                    case AdjecantDirections::BottomLeft:
                        Adjecent_BL = objectAtPos;
                        break;
                    case AdjecantDirections::BottomCenter:
                        Adjecent_BC = objectAtPos;
                        break;
                    case AdjecantDirections::BottomRight:
                        Adjecent_BR = objectAtPos;
                        break;
                    }
                }
            }
        }
	}

private:
    float TraceDistance = 2000;
    
    bool RaycastAdjecentObjects(int posX, int posY, FHitResult& result)  
    {
        FVector StartRaycastLocation = FVector(posX, posY, 1000);
        FVector DownwardVector = Parent->GetOwner()->GetActorUpVector() * -1;
        FVector EndLocation = StartRaycastLocation + DownwardVector * TraceDistance;
       
        FHitResult HitResult;
       
        bool bHit = Parent->GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, ECC_GameTraceChannel1);
       
        if (bHit)
        {
            result = HitResult;
            //DrawDebugLine(Parent->GetWorld(), StartRaycastLocation, EndLocation, FColor::Green, false, 3, 0, 1);
        }
        else
        {
            //DrawDebugLine(Parent->GetWorld(), StartRaycastLocation, EndLocation, FColor::Red, false, 3, 0, 1);
        }
        return bHit;
    }
   
     T* GetAdjecantObject(AdjecantDirections directionToGet) 
     {
         switch (directionToGet)
         {
         case AdjecantDirections::TopLeft:
             return Adjecent_TL;
             break;
         case AdjecantDirections::TopCenter:
             return Adjecent_TC;
             break;
         case AdjecantDirections::TopRight:
             return Adjecent_TR;
             break;
         case AdjecantDirections::Left:
             return Adjecent_L;
             break;
         case AdjecantDirections::Right:
             return Adjecent_R;
             break;
         case AdjecantDirections::BottomLeft:
             return Adjecent_BL;
             break;
         case AdjecantDirections::BottomCenter:
             return Adjecent_BC;
             break;
         case AdjecantDirections::BottomRight:
             return Adjecent_BR;
             break;
         }
         return nullptr;
     }

    GridPosition GetAdjecentPosition(AdjecantDirections directionToGet)
    {
        int ParentLocationX = Parent->GetComponentLocation().X;
        int ParentLocationY = Parent->GetComponentLocation().Y;
        switch (directionToGet)
        {
        case AdjecantDirections::TopLeft:
            return GridPosition(ParentLocationX + 105, ParentLocationY - 105);
            break;
        case AdjecantDirections::TopCenter:
            return GridPosition(ParentLocationX + 105, ParentLocationY);
            break;
        case AdjecantDirections::TopRight:
            return GridPosition(ParentLocationX + 105, ParentLocationY + 105);
            break;
        case AdjecantDirections::Left:
            return GridPosition(ParentLocationX, ParentLocationY - 105);
            break;
        case AdjecantDirections::Right:
            return GridPosition(ParentLocationX, ParentLocationY + 105);
            break;
        case AdjecantDirections::BottomLeft:
            return GridPosition(ParentLocationX - 105, ParentLocationY - 105);
            break;
        case AdjecantDirections::BottomCenter:
            return GridPosition(ParentLocationX - 105, ParentLocationY);
            break;
        case AdjecantDirections::BottomRight:
            return GridPosition(ParentLocationX - 105, ParentLocationY + 105);
            break;
        }
        return GridPosition(0, 0);
    }
};
