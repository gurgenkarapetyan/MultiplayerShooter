// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Controllers/LocalPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"

ALocalPlayerController::ALocalPlayerController()
{
	// set our turn rate for input
	TurnRateGamepad = TURN_RATE_GAMEPAD;	
}

void ALocalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	
	// PossessedCharacter = Cast<AUnrealTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	// check(PossessedCharacter);
}

void ALocalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up gameplay key bindings
	check(InputComponent);

	JumpBinding();
	
	MovementBinding();

	TurnBinding();
	LookUpBinding();
	
	FireBinding();
	ReloadBinding();
}

void ALocalPlayerController::JumpBinding()
{
	InputComponent->BindAction("Jump", IE_Pressed, this, &ALocalPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ALocalPlayerController::JumpStop);
}

void ALocalPlayerController::MovementBinding()
{
	InputComponent->BindAxis("Move Forward / Backward", this, &ALocalPlayerController::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &ALocalPlayerController::MoveRight);
}

void ALocalPlayerController::TurnBinding()
{
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &ALocalPlayerController::AddYawInput);
	InputComponent->BindAxis("Turn Right / Left Gamepad", this, &ALocalPlayerController::TurnAtRate);
}

void ALocalPlayerController::LookUpBinding()
{
	InputComponent->BindAxis("Look Up / Down Mouse", this, &ALocalPlayerController::AddPitchInput);
	InputComponent->BindAxis("Look Up / Down Gamepad", this, &ALocalPlayerController::LookUpAtRate);
}

void ALocalPlayerController::FireBinding()
{
	InputComponent->BindAction("Fire", IE_Pressed, this, &ALocalPlayerController::FireButtonPressed);
	InputComponent->BindAction("Fire", IE_Released, this, &ALocalPlayerController::FireButtonReleased);
}

void ALocalPlayerController::FireButtonPressed()
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	
	check(PossessedCharacter);
	check(PossessedCharacter->GetWeapon());
	PossessedCharacter->GetWeapon()->StartFire();
}

void ALocalPlayerController::FireButtonReleased()
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	
	PossessedCharacter->GetWeapon()->StopFire();
}

void ALocalPlayerController::ReloadBinding()
{
	InputComponent->BindAction("Reload", IE_Pressed, this, &ALocalPlayerController::Reload);
}

void ALocalPlayerController::Reload()
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	PossessedCharacter->GetEquippedWeapon()->Reload();
}

void ALocalPlayerController::TurnAtRate(float Rate)
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	// calculate delta for this frame from the rate information
	check(PossessedCharacter)
	check(GetWorld())
	PossessedCharacter->AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALocalPlayerController::LookUpAtRate(float Rate)
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	// calculate delta for this frame from the rate information
	check(PossessedCharacter)
	check(GetWorld())
	PossessedCharacter->AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALocalPlayerController::Jump()
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	PossessedCharacter->Jump();
}

void ALocalPlayerController::JumpStop()
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	PossessedCharacter->StopJumping();
}

void ALocalPlayerController::MoveForward(float Value)
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	if (Value != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		PossessedCharacter->AddMovementInput(Direction, Value);
	}
}

void ALocalPlayerController::MoveRight(float Value)
{
	PossessedCharacter = Cast<AUnrealTestCharacter>(GetPawn());
	if (!PossessedCharacter)
	{
		return;
	}
	if (Value != 0.0f)
	{
		// find out which way is right
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		PossessedCharacter->AddMovementInput(Direction, Value);
	}
}
