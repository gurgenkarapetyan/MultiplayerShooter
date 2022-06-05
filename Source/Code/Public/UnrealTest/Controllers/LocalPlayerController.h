// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LocalPlayerController.generated.h"

class AUnrealTestCharacter;
/**
 * 
 */
UCLASS()
class UNREALTEST_API ALocalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALocalPlayerController();
	
	void Jump();
	void JumpStop();
	void MovementBinding();
	void TurnBinding();
	void LookUpBinding();
	
	void FireButtonPressed();
	void FireButtonReleased();

	void Reload();
	
protected:
	virtual void BeginPlay() override;
	
	// APawn interface
	virtual void SetupInputComponent() override;
	void JumpBinding();
	// End of APawn interface

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	
	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	void FireBinding();

	void ReloadBinding();
	
protected:
	UPROPERTY()
	TObjectPtr<AUnrealTestCharacter> PossessedCharacter; 
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	float TurnRateGamepad;
	
	const float TURN_RATE_GAMEPAD = 50.f;
};
