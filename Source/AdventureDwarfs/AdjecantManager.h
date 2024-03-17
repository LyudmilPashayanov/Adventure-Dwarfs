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

	void SetAdjecantObjects(FVector componentUpVector, UWorld* componentWorld, FVector componentLocation, float componentWidth)
	{
        FHitResult hit;
        for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
        {
            AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
            GridPosition positionToCheck = GetAdjecentPosition(currentEnumValue, componentLocation, componentWidth); // get the size of the component
            if (RaycastAdjecentObjects(positionToCheck.X, positionToCheck.Y, hit, componentUpVector, componentWorld))
            {
                TArray<USceneComponent*> parents;
                hit.GetComponent()->GetParentComponents(parents);
                bool parentFound = false;
                for (USceneComponent* parent : parents)
                {
                    if (parent->IsA(T::StaticClass()))
                    {
                        UE_LOG(LogTemp, Log, TEXT("success 11111111111111111111111"));
                        T* objectAtPos = Cast<T>(parent);
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
                        parentFound=true;
                        break;
                    }
                }
                if(parentFound == false)
                {
                    if(hit.GetComponent()->GetOwner()->IsA(T::StaticClass()))
                    {
                        T* objectAtPos = Cast<T>(hit.GetComponent()->GetOwner());
                        UE_LOG(LogTemp, Log, TEXT("success 2222222222222222222"));
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
                    else
                    {
                        UE_LOG(LogTemp, Log, TEXT("failed to find anything"));
                    }
                }
            }
        }
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
private:
    float TraceDistance = 2000;
    
    bool RaycastAdjecentObjects(int posX, int posY, FHitResult& result, FVector componentUpVector, UWorld* componentWorld)
    {
        FVector StartRaycastLocation = FVector(posX, posY, 1000);
        FVector DownwardVector = componentUpVector * -1;
        FVector EndLocation = StartRaycastLocation + DownwardVector * TraceDistance;
       
        UE_LOG(LogTemp, Log, TEXT("raycasting :) "));

        FHitResult HitResult;
       
        bool bHit = componentWorld->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, T::TraceChannelValue);
        
        if (bHit)
        {
            result = HitResult;
            if(T::TraceChannelValue == ECC_GameTraceChannel1)
                DrawDebugLine(componentWorld, StartRaycastLocation, EndLocation, FColor::Green, false, 3, 0, 1);
        }
        else
        {
            if(T::TraceChannelValue == ECC_GameTraceChannel1)
                DrawDebugLine(componentWorld, StartRaycastLocation, EndLocation, FColor::Red, false, 3, 0, 1);
        }
        return bHit;
    }
    

    GridPosition GetAdjecentPosition(AdjecantDirections directionToGet, FVector componentLocation, float componentWidth)
    {
        int ParentLocationX = componentLocation.X;
        int ParentLocationY = componentLocation.Y;
        int halfSize = componentWidth;
        switch (directionToGet)
        {
        case AdjecantDirections::TopLeft:
            return GridPosition(ParentLocationX + halfSize, ParentLocationY - halfSize);
            break;
        case AdjecantDirections::TopCenter:
            return GridPosition(ParentLocationX + halfSize, ParentLocationY);
            break;
        case AdjecantDirections::TopRight:
            return GridPosition(ParentLocationX + halfSize, ParentLocationY + halfSize);
            break;
        case AdjecantDirections::Left:
            return GridPosition(ParentLocationX, ParentLocationY - halfSize);
            break;
        case AdjecantDirections::Right:
            return GridPosition(ParentLocationX, ParentLocationY + halfSize);
            break;
        case AdjecantDirections::BottomLeft:
            return GridPosition(ParentLocationX - halfSize, ParentLocationY - halfSize);
            break;
        case AdjecantDirections::BottomCenter:
            return GridPosition(ParentLocationX - halfSize, ParentLocationY);
            break;
        case AdjecantDirections::BottomRight:
            return GridPosition(ParentLocationX - halfSize, ParentLocationY + halfSize);
            break;
        }
        return GridPosition(0, 0);
    }
};
