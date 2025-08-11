// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTextActor.h"

// Sets default values
ADebugTextActor::ADebugTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void ADebugTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShowingDebug)
	{
		DrawDebugString(GetWorld(), DebugLocation, FString::Printf(TEXT("%s"), *CurrentDebugText), this, FColor::Red, 0.1f,false, 1);
	}
}

void ADebugTextActor::ShowDebugString(const FString& Text, const FVector& Location)
{
	bShowingDebug = true;
	CurrentDebugText = Text;
	DebugLocation = Location;
}

void ADebugTextActor::ClearDebugString()
{
	bShowingDebug=false;
}

