// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Engine/DataTable.h"
#include "Chunk.generated.h"

class UCell;
struct ConstructorHelpers;
class FObjectFinder;
class UDataTable;
template<class T>
class AdjecantManager;

UCLASS()
class ADVENTUREDWARFS_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	//AdjecantManager<AChunk>* Adjecants;
	
	// Sets default values for this actor's properties
	AChunk();

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TArray<UStaticMeshComponent*> StaticMeshComponents; // TODO: Remove this and use the UCell static mesh if u need it.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TArray<UCell*> Cells;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* FloatCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSoftObjectPtr<UDataTable> ChunkJsonData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TArray<UBoxComponent*> BoxColliders;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual ConstructorHelpers::FObjectFinder<UDataTable> GetGridConstructJsonPath();
private:	
	void OnCellStepped(UCell* Cell);
	void ConstructCell(int CellIndex, FVector Translation, FRotator Rotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitializeCells();
	void Hide();
};
