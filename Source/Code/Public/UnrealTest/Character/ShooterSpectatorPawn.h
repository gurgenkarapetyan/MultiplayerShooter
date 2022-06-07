// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "ShooterSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AShooterSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	AShooterSpectatorPawn(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void LookUpAtRate(float Val) override;
};
