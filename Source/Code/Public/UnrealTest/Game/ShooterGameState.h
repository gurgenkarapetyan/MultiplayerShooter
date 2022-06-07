// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ShooterGameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AShooterGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AShooterGameState();

	FORCEINLINE int32 GetTeamsNumber() const { return TeamsNumber; }
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	/** Returns match scores */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<int32> GetTeamScores() const; 
	
	UPROPERTY(Replicated)
	TArray<int32> TeamScores;
	
private:
	UPROPERTY(Replicated)
	int32 TeamsNumber;
};
