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

	void SetAdjecantObjects() 
	{
        //FHitResult hit;
        //for (int i = 0; i < static_cast<int>(AdjecantDirections::Count); ++i)
        //{
        //    AdjecantDirections currentEnumValue = static_cast<AdjecantDirections>(i);
        //    GridPosition positionToCheck = GetAdjecentPosition(currentEnumValue);
        //    if (RaycastAdjecentObjects(positionToCheck.X, positionToCheck.Y, hit))
        //    {
        //        USceneComponent* ObjectSceneComponent = hit.GetComponent()->GetChildComponent(0);
        //        if (ObjectSceneComponent->IsA(T::StaticClass())) // maybe not very flexible :P 
        //        {
        //            // UE_LOG(LogTemp, Log, TEXT("hit CHILD INDEX 0 name: %s"), *CellSceneComponent->GetName());
        //            T* objectAtPos = Cast<T>(ObjectSceneComponent);
        //            switch (currentEnumValue)
        //            {
        //            case AdjecantDirections::TopLeft:
        //                Adjecent_TL = objectAtPos;
        //                break;
        //            case AdjecantDirections::TopCenter:
        //                Adjecent_TC = objectAtPos;
        //                break;
        //            case AdjecantDirections::TopRight:
        //                Adjecent_TR = objectAtPos;
        //                break;
        //            case AdjecantDirections::Left:
        //                Adjecent_L = objectAtPos;
        //                break;
        //            case AdjecantDirections::Right:
        //                Adjecent_R = objectAtPos;
        //                break;
        //            case AdjecantDirections::BottomLeft:
        //                Adjecent_BL = objectAtPos;
        //                break;
        //            case AdjecantDirections::BottomCenter:
        //                Adjecent_BC = objectAtPos;
        //                break;
        //            case AdjecantDirections::BottomRight:
        //                Adjecent_BR = objectAtPos;
        //                break;
        //            }
        //        }
        //    }
        //}
	}

private:
	bool RaycastAdjecentObjects(int posX, int posY, FHitResult& result);
	T* GetAdjecantObject(AdjecantDirections directionToGet);
	GridPosition GetAdjecentPosition(AdjecantDirections directionToGet);
};
