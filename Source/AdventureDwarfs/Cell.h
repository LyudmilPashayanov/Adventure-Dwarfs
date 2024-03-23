// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "Cell.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FCellEvent, UCell*);

enum class AdjecantDirections;
class GridPosition;
class UCurveFloat;

template<class T>
class AdjecantManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUREDWARFS_API UCell : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCell();
	
	AdjecantManager<UCell>* Adjecants;
	
	static constexpr ECollisionChannel TraceChannelValue = ECC_GameTraceChannel1; // Custom trace channel

	FCellEvent CellSteppedEvent;
	bool ShouldRaycast;
	
	UStaticMeshComponent* CellMesh;
	FVector originalLocation;
private:	
	FTimeline MyTimeline;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		

	//void OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void PrintLocation();
	UCell* GetAdjecentCell(AdjecantDirections directionToGet);
	void SetAdjacentCells();
	void ShowAdjacentCells(int depth, UCurveFloat* curveFloat);
	
	void ShowCell(UCurveFloat* floatCurve);

	UFUNCTION()
	void TimelineCallback(float Value);
	UFUNCTION()
	void TimelineFinishedCallback();

	void HideCell();
	GridPosition GetAdjecentPosition(AdjecantDirections directionToGet);
};
