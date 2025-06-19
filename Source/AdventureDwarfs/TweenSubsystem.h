// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTweenTask.h"
#include "Subsystems/WorldSubsystem.h"
#include "TweenSubsystem.generated.h"

UCLASS()
class ADVENTUREDWARFS_API UTweenSubsystem : public UWorldSubsystem, public FTickableGameObject
{
GENERATED_BODY()

public:
    virtual void Tick(float DeltaTime) override;
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }
    virtual TStatId GetStatId() const override;
    virtual bool IsTickable() const override { return true; }
    virtual bool IsTickableInEditor() const override { return false; }
    void AddTween(FTweenTask&& Tween);

private:
    TArray<FTweenTask> ActiveTweens;	
};
