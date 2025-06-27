// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAdjacentDirection : uint8
{
	TopLeft      UMETA(DisplayName = "Top Left"),
	TopCenter    UMETA(DisplayName = "Top Center"),
	TopRight     UMETA(DisplayName = "Top Right"),
	Left         UMETA(DisplayName = "Left"),
	Right        UMETA(DisplayName = "Right"),
	BottomLeft   UMETA(DisplayName = "Bottom Left"),
	BottomCenter UMETA(DisplayName = "Bottom Center"),
	BottomRight  UMETA(DisplayName = "Bottom Right")
};