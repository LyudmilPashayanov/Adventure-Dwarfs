// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureDwarfsCharacter.h"

#include "Chunk.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Raycaster.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"


//////////////////////////////////////////////////////////////////////////
// AAdventureDwarfsCharacter

AAdventureDwarfsCharacter::AAdventureDwarfsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bCanEverTick = true;
}

void AAdventureDwarfsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAdventureDwarfsCharacter::GetPlayerGridPosition()
{
	FVector PlayerWorldPos = GetActorLocation(); // or wherever your character is

	// 2. Define chunk and cell sizes
	const int32 ChunkSizeWorld = 2000;
	const int32 CellsPerChunk = 20;
	const int32 CellSize = ChunkSizeWorld / CellsPerChunk; // 100 units

	const float HalfChunkSize = static_cast<float>(ChunkSizeWorld) / 2.0f;         // e.g. 1000.0f

	// Shift the world position so that (0,0) is at the top-left corner of the first chunk
	float ShiftedX = PlayerWorldPos.X + HalfChunkSize;
	float ShiftedY = PlayerWorldPos.Y + HalfChunkSize;

	// Convert to local cell index and +1 to avoid zero-based indexing
	int32 CellColumn = FMath::FloorToInt(ShiftedX / CellSize) + 1;
	int32 CellRow    = FMath::FloorToInt(ShiftedY / CellSize) + 1;

	FIntPoint GlobalCellCoord(CellColumn, CellRow);
	OnPlayerPositionChanged.Broadcast(GlobalCellCoord);
	//UE_LOG(LogTemp, Log, TEXT("GlobalCellCoord player position cell X: %d and Y: %d "), GlobalCellCoord.X, GlobalCellCoord.Y);
}

void AAdventureDwarfsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//RaycastTerrain();

	//TimeSinceLastGridCheck += DeltaSeconds;
	//if (TimeSinceLastGridCheck >= GridCheckInterval)
	{
	//	TimeSinceLastGridCheck = 0.0f; // reset timer
		GetPlayerGridPosition();       // call your grid position logic here
	}
}

void AAdventureDwarfsCharacter::RaycastTerrain()
{
	/*//Raycast on the terrain
	const FVector StartRaycastLocation = FVector(GetTransform().GetLocation().X, GetTransform().GetLocation().Y,GetTransform().GetLocation().Z);
	const FVector EndLocation = StartRaycastLocation - FVector(0,0,250);
	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartRaycastLocation, EndLocation, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(20));
	if(bHit)
	{
		if (CurrentChunk)
		{
			UHierarchicalInstancedStaticMeshComponent* HISMComponent = Cast<UHierarchicalInstancedStaticMeshComponent>(HitResult.Component);
			if (HISMComponent)
			{
				// Get instance index
				int32 HitInstanceIndex = HitResult.Item;

				//UE_LOG(LogTemp, Log, TEXT("Hit instance index: %d"), HitInstanceIndex);
				CurrentChunk->ShowCellByIndex(HitInstanceIndex);
			}
		}
	}
	else
	{
		DrawDebugSphere(GetWorld(),(StartRaycastLocation + EndLocation) / 2.0f, 20.0f, 12, FColor::Red,false,1);
	}*/
}


void AAdventureDwarfsCharacter::InjectRaycaster(URaycaster* raycastClass)
{
	RaycastChecker = raycastClass;
	raycastClass->OnCollectionStarted.AddUObject(this, &AAdventureDwarfsCharacter::StartCollecting_Handler);
	raycastClass->OnCollectionStopped.AddUObject(this, &AAdventureDwarfsCharacter::StopCollecting_Handler);
}

// Input
void AAdventureDwarfsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdventureDwarfsCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdventureDwarfsCharacter::Look);	
		
		//Interacting
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &AAdventureDwarfsCharacter::StartInteractAction);
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Completed, this, &AAdventureDwarfsCharacter::StopInteractAction);

	}

}

void AAdventureDwarfsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAdventureDwarfsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAdventureDwarfsCharacter::StartInteractAction(const FInputActionValue& Value)
{
	bool clicked = Value.Get<bool>();
	if(clicked)
	{
		//TODO: Depending on what you have equipped or not in your left item slot do a specific action:

		
		// TODO:Case: Nothing/Pickaxe equipped
		RaycastChecker->CheckFacingObject(); // TODO: This has to be the pick-axe item itself
		
		//UE_LOG(LogTemp, Log, TEXT("StartInteractAction"));
	}
}

void AAdventureDwarfsCharacter::StopInteractAction(const FInputActionValue& Value)
{
	bool clicked = Value.Get<bool>();
	if(clicked == false)
	{
		RaycastChecker->StopUse();
		//UE_LOG(LogTemp, Log, TEXT("StopInteractAction"));
	}
}

void AAdventureDwarfsCharacter::StartCollecting_Handler(float collectTime)
{
	UE_LOG(LogTemp, Log, TEXT("collectTime: %f"), collectTime);
	StartCollectionEvent(collectTime);
}

void AAdventureDwarfsCharacter::StopCollecting_Handler()
{
	StopCollectingEvent();
}






