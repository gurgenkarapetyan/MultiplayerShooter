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
	
	PossessedCharacter = Cast<AUnrealTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(PossessedCharacter);
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

void ALocalPlayerController::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	PossessedCharacter->AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALocalPlayerController::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	PossessedCharacter->AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALocalPlayerController::Jump()
{
	PossessedCharacter->Jump();
}

void ALocalPlayerController::JumpStop()
{
	PossessedCharacter->StopJumping();
}

void ALocalPlayerController::MoveForward(float Value)
{
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
