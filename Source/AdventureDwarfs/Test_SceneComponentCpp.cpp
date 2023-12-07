// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_SceneComponentCpp.h"

// Sets default values for this component's properties
UTest_SceneComponentCpp::UTest_SceneComponentCpp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTest_SceneComponentCpp::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTest_SceneComponentCpp::PrintLocation()
{
	UE_LOG(LogTemp, Log, TEXT("current position is: x- %f , y- %f , z- %f"), GetComponentLocation().X, GetComponentLocation().Y, GetComponentLocation().Z);
}

// Called every frame
void UTest_SceneComponentCpp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

