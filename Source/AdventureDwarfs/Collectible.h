// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Collectible.generated.h"

class UCell;
class UCollectibleDataAsset;

UCLASS()
class ADVENTUREDWARFS_API ACollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectible();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FString Orientation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	FVector Size;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TArray<UCell*> ParentCells;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "UI Custom Components")
	UWidgetComponent* WidgetComponent;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float collectTimeRequired = 2;
	
public:
	void Init(UCollectibleDataAsset* data);
	void NotifyParentsShow();
	void Collect();
	float GetCollectTimeRequired() const;
};
