// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugTextActor.generated.h"

UCLASS()
class ADVENTUREDWARFS_API ADebugTextActor : public AActor
{
	GENERATED_BODY()
    
public:
	ADebugTextActor();
	virtual void Tick(float DeltaTime) override;
    
	void ShowDebugString(const FString& Text, const FVector& Location);
	void ClearDebugString();
    
private:
	bool bShowingDebug = false;
	FString CurrentDebugText;
	FVector DebugLocation;

};
