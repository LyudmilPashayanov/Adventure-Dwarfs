// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Raycaster.generated.h"


class ACollectible;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCollectingSignature, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUREDWARFS_API URaycaster : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URaycaster();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float TraceDistance = 100;
	ACollectible* CurrentInteractable;
	float HoldStartTime;
	bool bIsBeingCollected;
	FTimerHandle HoldTimerHandle;
	void CollectingUpdate();
	
public:
	FOnCollectingSignature OnInteractableBeingCollected;
	void CheckFacingObject();
	void StopUse();
};
