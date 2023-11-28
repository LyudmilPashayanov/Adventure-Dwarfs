// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Cell.generated.h"

class UBoxComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FCellEvent, ACell*);


UCLASS()
class ADVENTUREDWARFS_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();
	int posX;
	int posY;
	ACell* Adjecent_TL; // top left adjecent cell
	ACell* Adjecent_TC; // top center adjecent cell
	ACell* Adjecent_TR; // top right adjecent cell
	ACell* Adjecent_L;  // left adjecent cell
	ACell* Adjecent_R;  // right adjecent cell
	ACell* Adjecent_BL; // bottom left adjecent cell
	ACell* Adjecent_BC; // bottom center adjecent cell
	ACell* Adjecent_BR; // bottom right adjecent cell
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Components")
	UBoxComponent* BoxOverlapComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Components")
	UStaticMeshComponent* MeshComponent;
	
	void Init(int PosX, int PosY);
};
