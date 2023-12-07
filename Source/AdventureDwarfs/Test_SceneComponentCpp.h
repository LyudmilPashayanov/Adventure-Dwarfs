// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Test_SceneComponentCpp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUREDWARFS_API UTest_SceneComponentCpp : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTest_SceneComponentCpp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
