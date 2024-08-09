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
    AdjecantManager(float ComponentWidth, FVector ComponentLocation)
    {
        componentWidth = ComponentWidth;
        componentLocation = ComponentLocation;
    }
    
	UPROPERTY()
	T* Adjacent_TL = nullptr; // top left adjacent
	UPROPERTY()
	T* Adjacent_TC = nullptr; // top center adjacent
	UPROPERTY()
	T* Adjacent_TR = nullptr; // top right adjacent 
	UPROPERTY()
	T* Adjacent_L = nullptr;  // left adjacent
	UPROPERTY()
	T* Adjacent_R = nullptr;  // right adjacent 
	UPROPERTY()
	T* Adjacent_BL = nullptr; // bottom left adjacent
	UPROPERTY()
	T* Adjacent_BC = nullptr; // bottom center adjacent
	UPROPERTY()
	T* Adjacent_BR = nullptr; // bottom right adjacent 

	
	void SetAdjacentObjects(FVector componentUpVector, UWorld* componentWorld)
	{
        FHitResult hit;
        for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
        {
            AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
            GridPosition positionToCheck = GetAdjacentPosition(currentEnumValue);
            if (RaycastAdjacentObjects(positionToCheck.X, positionToCheck.Y, hit, componentUpVector, componentWorld))
            {
            	if(hit.GetComponent()->GetOwner()->IsA(T::StaticClass()))
                {
                    T* objectAtPos = Cast<T>(hit.GetComponent()->GetOwner());
                	SetAdjacent(currentEnumValue, objectAtPos);
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("failed to find any adjacent object "));
                }
            }
        }
	}
	
	void SetAdjacent(AdjecantDirections directionToSet, T* ObjectToSet)
	{
		switch (directionToSet)
		{
		case AdjecantDirections::TopLeft:
			Adjacent_TL = ObjectToSet;
			break;
		case AdjecantDirections::TopCenter:
			Adjacent_TC = ObjectToSet;
			break;
		case AdjecantDirections::TopRight:
			Adjacent_TR = ObjectToSet;
			break;
		case AdjecantDirections::Left:
			Adjacent_L = ObjectToSet;
			break;
		case AdjecantDirections::Right:
			Adjacent_R = ObjectToSet;
			break;
		case AdjecantDirections::BottomLeft:
			Adjacent_BL = ObjectToSet;
			break;
		case AdjecantDirections::BottomCenter:
			Adjacent_BC = ObjectToSet;
			break;
		case AdjecantDirections::BottomRight:
			Adjacent_BR = ObjectToSet;
			break;
		}
	}
	
    T* GetAdjacentObject(AdjecantDirections directionToGet)
    {
        switch (directionToGet)
        {
        case AdjecantDirections::TopLeft:
            return Adjacent_TL;
        case AdjecantDirections::TopCenter:
            return Adjacent_TC;
        case AdjecantDirections::TopRight:
            return Adjacent_TR;
        case AdjecantDirections::Left:
            return Adjacent_L;
        case AdjecantDirections::Right:
            return Adjacent_R;
        case AdjecantDirections::BottomLeft:
            return Adjacent_BL;
        case AdjecantDirections::BottomCenter:
            return Adjacent_BC;
        case AdjecantDirections::BottomRight:
            return Adjacent_BR;
        }
        return nullptr;
    }
	
    GridPosition GetAdjacentPosition(AdjecantDirections directionToGet)
	{
	    const int ParentLocationX = componentLocation.X;
	    const int ParentLocationY = componentLocation.Y;
	    const int halfSize = componentWidth;
	    switch (directionToGet)
	    {
	    case AdjecantDirections::TopLeft:
	        return GridPosition(ParentLocationX + halfSize, ParentLocationY - halfSize);
	    case AdjecantDirections::TopCenter:
	        return GridPosition(ParentLocationX + halfSize, ParentLocationY);
	    case AdjecantDirections::TopRight:
	        return GridPosition(ParentLocationX + halfSize, ParentLocationY + halfSize);
	    case AdjecantDirections::Left:
	        return GridPosition(ParentLocationX, ParentLocationY - halfSize);
	    case AdjecantDirections::Right:
	        return GridPosition(ParentLocationX, ParentLocationY + halfSize);
	    case AdjecantDirections::BottomLeft:
	        return GridPosition(ParentLocationX - halfSize, ParentLocationY - halfSize);
	    case AdjecantDirections::BottomCenter:
	        return GridPosition(ParentLocationX - halfSize, ParentLocationY);
	    case AdjecantDirections::BottomRight:
	        return GridPosition(ParentLocationX - halfSize, ParentLocationY + halfSize);
	    }
	    return GridPosition(0, 0);
	}
	
private:
    
    float TraceDistance = 2000;
    float componentWidth;
    FVector componentLocation;
    
    bool RaycastAdjacentObjects(int posX, int posY, FHitResult& result, FVector componentUpVector, UWorld* componentWorld)
    {
        FVector StartRaycastLocation = FVector(posX, posY, 1000); 
        FVector DownwardVector = componentUpVector * -1;
        FVector EndLocation = StartRaycastLocation + DownwardVector * TraceDistance;
       
        //UE_LOG(LogTemp, Log, TEXT("raycasting :) "));

        FHitResult HitResult;
       
        bool bHit = componentWorld->LineTraceSingleByChannel(HitResult, StartRaycastLocation, EndLocation, ECC_GameTraceChannel3);
        
        if (bHit)
        {
            result = HitResult;
                //DrawDebugLine(componentWorld, StartRaycastLocation, EndLocation, FColor::Green, false, 3, 0, 1);
        }
        else
        {
                //DrawDebugLine(componentWorld, StartRaycastLocation, EndLocation, FColor::Red, false, 3, 0, 1);
        }
        return bHit;
    }
};
