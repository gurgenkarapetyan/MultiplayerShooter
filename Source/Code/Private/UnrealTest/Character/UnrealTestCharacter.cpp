// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AUnrealTestCharacter

AUnrealTestCharacter::AUnrealTestCharacter()
	: CarriedAmmo(85)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	DisableCotrollerRotation();

	ConfigureCharacterMovement(GetCharacterMovement());
	
	SetCameraBoom();
	SetFollowCamera();

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AUnrealTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	if (GetLocalRole() == ROLE_Authority && Weapon_BP)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
	
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(Weapon_BP, SpawnParameters);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));

		HealthComponent->OnHealthChanged.AddDynamic(this, &AUnrealTestCharacter::OnHealthChanged);
	}
}

void AUnrealTestCharacter::DisableCotrollerRotation()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AUnrealTestCharacter::ConfigureCharacterMovement(UCharacterMovementComponent* characterMovement)
{
	// Configure character movement
	characterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	characterMovement->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	characterMovement->JumpZVelocity = JUMP_Z_VELOCITY;
	characterMovement->AirControl = AIR_CONTROL;
	characterMovement->MaxWalkSpeed = MAX_WALK_SPEED;
	characterMovement->MinAnalogWalkSpeed = MIN_ANALOG_WALK_SPEED;
	characterMovement->BrakingDecelerationWalking = BRAKING_DECELERATION_WALKING;
}

void AUnrealTestCharacter::SetCameraBoom()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
}

void AUnrealTestCharacter::SetFollowCamera()
{
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AUnrealTestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnrealTestCharacter, Weapon);
}

void AUnrealTestCharacter::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageTyp, FName BoneName, AController* InstigatedBy, AActor* DamageCauser)
{
	
}
