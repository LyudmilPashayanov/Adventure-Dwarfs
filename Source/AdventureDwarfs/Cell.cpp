// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include "AdventureDwarfsCharacter.h"
#include <Components/BoxComponent.h>

// Sets default values
ACell::ACell()
{


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxComponent;
	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxOverlapDetector"));
	BoxOverlapComponent->SetupAttachment(RootComponent);
	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ACell::OnBeginOverlap);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}
// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACell::Init(int PosX, int PosY)
{
	posX = PosX;
	posY = PosY;
}

void ACell::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->IsA(AAdventureDwarfsCharacter::StaticClass()))
	{
		CellSteppedEvent.Broadcast(this);
	}
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

