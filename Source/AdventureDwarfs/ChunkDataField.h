// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"  // Include the DataTable header

#include "ChunkDataField.generated.h"


/** Please add a struct description */
USTRUCT(BlueprintType)
struct FChunkDataField : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "number"))
	int32 cell_index;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "mesh_name", MakeStructureDefaultValue = "defaultCell"))
	FString mesh_name;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "translation", MakeStructureDefaultValue = "(0,0,0)"))
	TArray<int32> translation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "rotation", MakeStructureDefaultValue = "(0,0,0)"))
	TArray<int32> rotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "scale", MakeStructureDefaultValue = "(0,0,0)"))
	TArray<int32> scale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "row", MakeStructureDefaultValue = "-1"))
	int32 row;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "column", MakeStructureDefaultValue = "-1"))
	int32 column;
};

