// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Engine/DataTable.h"
#include "Containers/Map.h"

#include "Chunk.generated.h"

class UCell;
struct ConstructorHelpers;
class FObjectFinder;
class UDataTable;
template<class T>
class AdjecantManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChunkStepped, AChunk*);
UCLASS()

class ADVENTUREDWARFS_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:

	AdjecantManager<AChunk>* AdjecantsManager;
	static constexpr ECollisionChannel TraceChannelValue = ECC_GameTraceChannel3; // Custom trace channel
	FOnChunkStepped OnChunkStepped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	UBoxComponent* ChunkOverlapComponent;
	// Sets default values for this actor's properties
	AChunk();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TArray<UCell*> Cells;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Attribute")
	TMap<FString, UCell*> LocationCellPairs;
	
	UPROPERTY(EditAnywhere)
	class UCurveFloat* FloatCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSoftObjectPtr<UDataTable> ChunkJsonData;



	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual ConstructorHelpers::FObjectFinder<UDataTable> GetGridConstructJsonPath();
private:	
	void OnCellStepped(UCell* Cell);
	void ConstructCell(int CellIndex, FVector Translation, FRotator Rotation, UHierarchicalInstancedStaticMeshComponent* StaticMeshInstance, int row, int column);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitializeCells();
	void SetAdjacents();
	void Show();
};
