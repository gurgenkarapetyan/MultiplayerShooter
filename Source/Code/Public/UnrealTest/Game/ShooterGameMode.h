// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UnrealTest/Character/ShooterPlayerState.h"
#include "ShooterGameMode.generated.h"


class APlayerStart;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchEnd);

/**
 * 
 */
UCLASS()
class UNREALTEST_API AShooterGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AShooterGameMode(const FObjectInitializer& ObjectInitializer);
	
	/** Select best spawn point for player */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/** Checks Player State team in spawning point */ 
	virtual bool CheckStartTeam(APlayerStart* PlayerStart, AController* Player) const;

	/** New player joins */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	/** Set team randomly */
	int32 ChooseTeam(AShooterPlayerState* PlayerState) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 PlayerNumberToStartGame;
	
	int32 NumTeams;

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true"))
	FOnMatchStart OnMatchStart;

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true"))
	FOnMatchEnd OnMatchEnd;
};
