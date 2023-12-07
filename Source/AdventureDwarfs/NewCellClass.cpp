// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCellClass.h"
#include "Test_SceneComponentCpp.h"

// Sets default values for this component's properties
UNewCellClass::UNewCellClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNewCellClass::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Log, TEXT("name: %s"), *GetAttachParent()->GetChildComponent(3)->GetName());

	AActor* owner = GetOwner();
	if (owner) {
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		owner->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
		UE_LOG(LogTemp, Log, TEXT("count: %d"), StaticMeshComponents.Num());

		for (UStaticMeshComponent* MeshComponent : StaticMeshComponents)
		{
			UTest_SceneComponentCpp* MyComponentInstance = NewObject<UTest_SceneComponentCpp>(this);
			MyComponentInstance->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
			MyComponentInstance->PrintLocation();
		}
	}
}


// Called every frame
void UNewCellClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

