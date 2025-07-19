#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class UCell;
class UCollectibleDataAsset;
class AChunk;

UCLASS()
class ADVENTUREDWARFS_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	virtual void BeginPlay() override;
	
private:
	void ChunkStepped_Handler(AChunk* SteppedChunk);
	void SpawnAdjacentChunks(const AChunk* ChunkToSpawnAround);
	void SetupCollectibles(AChunk* ChunkToSetup);
	void RevealCellsAroundPlayer(const FIntPoint& playerPosition);

	TArray<FIntPoint> PlayerRevealRadius;
	
public:
	
	TMap<FIntPoint, TArray<UCell*>> GlobalCellsMap2D; // Map used to reveal nearby cells when players walks
	TMap<FIntVector, UCell*> GlobalCellsMap3D; // Map used for pathfinding.
	TMap<FIntPoint, AChunk*> GlobalChunkMap;
	
	UPROPERTY(EditAnywhere, Category = "Cell Manager")
	TArray<TSubclassOf<class AChunk>> ChunksLandforms;
	void GenerateGrid();
	void AddCellToMap(FIntPoint coordinates2D, FIntVector coordinates3D, UCell* cell);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectibles Collection", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACollectible> BaseCollectible;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectibles Collection", meta = (AllowPrivateAccess = true))
	TArray<UCollectibleDataAsset*> CollectiblesData;
	AChunk* SpawnChunk(FIntPoint position, bool hidden);
};
