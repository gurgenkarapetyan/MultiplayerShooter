// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/ShooterGameState.h"

#include "Net/UnrealNetwork.h"

AShooterGameState::AShooterGameState()
{
	UE_LOG(LogTemp, Warning, TEXT("AShooterGameState::AUT_DeathMatchGameState"));

	TeamsNumber = 2;
	TeamScores.AddZeroed(TeamsNumber);
}

void AShooterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UE_LOG(LogTemp, Warning, TEXT("AShooterGameState::GetLifetimeReplicatedProps"));

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterGameState, TeamsNumber);
	DOREPLIFETIME(AShooterGameState, TeamScores);
}

TArray<int32> AShooterGameState::GetTeamScores() const
{
	UE_LOG(LogTemp, Warning, TEXT("AShooterGameState::GetTeamScores"));

	return TeamScores;
}