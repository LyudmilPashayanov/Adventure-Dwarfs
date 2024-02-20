// Fill out your copyright notice in the Description page of Project Settings.


#include "AdjecantManager.h"
#include "AdjecantDirections.h"
#include "GridPosition.h"
#include "Raycaster.h"

//template<class T>
//void AdjecantManager<T>::SetAdjecantObjects()
//{
// 
//}

template<class T>
GridPosition AdjecantManager<T>::GetAdjecentPosition(AdjecantDirections directionToGet)
{
    switch (directionToGet)
    {
    case AdjecantDirections::TopLeft:
        return GridPosition(GetComponentLocation().X + 105, GetComponentLocation().Y - 105);
        break;
    case AdjecantDirections::TopCenter:
        return GridPosition(GetComponentLocation().X + 105, GetComponentLocation().Y);
        break;
    case AdjecantDirections::TopRight:
        return GridPosition(GetComponentLocation().X + 105, GetComponentLocation().Y + 105);
        break;
    case AdjecantDirections::Left:
        return GridPosition(GetComponentLocation().X, GetComponentLocation().Y - 105);
        break;
    case AdjecantDirections::Right:
        return GridPosition(GetComponentLocation().X, GetComponentLocation().Y + 105);
        break;
    case AdjecantDirections::BottomLeft:
        return GridPosition(GetComponentLocation().X - 105, GetComponentLocation().Y - 105);
        break;
    case AdjecantDirections::BottomCenter:
        return GridPosition(GetComponentLocation().X - 105, GetComponentLocation().Y);
        break;
    case AdjecantDirections::BottomRight:
        return GridPosition(GetComponentLocation().X - 105, GetComponentLocation().Y + 105);
        break;
    }
    return GridPosition(0, 0);
}

template<class T>
T* AdjecantManager<T>::GetAdjecantObject(AdjecantDirections directionToGet)
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

template<class T>
bool AdjecantManager<T>::RaycastAdjecentObjects(int posX, int posY, FHitResult& result)
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