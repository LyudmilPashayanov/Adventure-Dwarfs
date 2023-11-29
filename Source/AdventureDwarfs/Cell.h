// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridPosition.h"
#include "Cell.generated.h"

class UBoxComponent;
class URaycaster;
enum class AdjecentDirections;

DECLARE_MULTICAST_DELEGATE_OneParam(FCellEvent, ACell*);


UCLASS()
class ADVENTUREDWARFS_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();
	GridPosition pos;
	ACell* Adjecent_TL = nullptr; // top left adjecent cell
	ACell* Adjecent_TC = nullptr; // top center adjecent cell
	ACell* Adjecent_TR = nullptr; // top right adjecent cell
	ACell* Adjecent_L = nullptr;  // left adjecent cell
	ACell* Adjecent_R = nullptr;  // right adjecent cell
	ACell* Adjecent_BL = nullptr; // bottom left adjecent cell
	ACell* Adjecent_BC = nullptr; // bottom center adjecent cell
	ACell* Adjecent_BR = nullptr; // bottom right adjecent cell
	FCellEvent CellSteppedEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	URaycaster* RaycastChecker;
	UFUNCTION(BlueprintCallable, Category = "Raycaster")
	void InjectRaycaster(URaycaster* raycastClass);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Components")
	UBoxComponent* BoxOverlapComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Components")
	UStaticMeshComponent* MeshComponent;
	
	void Init(GridPosition position);
	bool CheckAdjecentCell(AdjecentDirections directionToCheck);
	GridPosition GetAdjecentPosition(AdjecentDirections directionToGet);
	ACell* GetAdjecentCell(AdjecentDirections directionToGet);
};
