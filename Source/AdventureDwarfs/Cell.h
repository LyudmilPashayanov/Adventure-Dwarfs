// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Cell.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FCellEvent, UCell*);

enum class AdjecentDirections;
class GridPosition;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUREDWARFS_API UCell : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCell();

	FCellEvent CellSteppedEvent;
	bool ShouldRaycast;
	
	UPROPERTY()
	UCell* Adjecent_TL = nullptr; // top left adjecent cell
	UPROPERTY()
	UCell* Adjecent_TC = nullptr; // top center adjecent cell
	UPROPERTY()
	UCell* Adjecent_TR = nullptr; // top right adjecent cell
	UPROPERTY()
	UCell* Adjecent_L = nullptr;  // left adjecent cell
	UPROPERTY()
	UCell* Adjecent_R = nullptr;  // right adjecent cell
	UPROPERTY()
	UCell* Adjecent_BL = nullptr; // bottom left adjecent cell
	UPROPERTY()
	UCell* Adjecent_BC = nullptr; // bottom center adjecent cell
	UPROPERTY()
	UCell* Adjecent_BR = nullptr; // bottom right adjecent cell

	UStaticMeshComponent* CellMesh;

private:	
	float TraceDistance = 100;
	bool RaycastAdjecentCells(int posX, int posY, FHitResult& HitResult);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PrintLocation();
	UCell* GetAdjecentCell(AdjecentDirections directionToGet);
	void SetAdjecentCells();
	void ShowAdjecentCells(int depth);
	void ShowCell();
	void HideCell();
	GridPosition GetAdjecentPosition(AdjecentDirections directionToGet);
};
