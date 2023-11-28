// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureDwarfsGameMode.h"
#include "AdventureDwarfsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAdventureDwarfsGameMode::AAdventureDwarfsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
