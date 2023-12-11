// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "Chunk.generated.h"

class UCell;
UCLASS()
class ADVENTUREDWARFS_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute", meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*> StaticMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute", meta = (AllowPrivateAccess = true))
	TArray<UCell*> Cells;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute", meta = (AllowPrivateAccess = true))
	TArray<UBoxComponent*> BoxColliders;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void OnCellEventReceived(UCell* Cell);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
