// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <deque>
#include "GridManager.generated.h"


class ACell;

UCLASS()
class ADVENTUREDWARFS_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();
	std::deque<std::deque<ACell*>> Grid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void OnCellEventReceived(ACell* Cell);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Cell Manager")
	TSubclassOf<class ACell> CellPrefab;
	void GenerateGrid(int rows, int columns);
	ACell* SpawnCell(int posX, int posY);
};
