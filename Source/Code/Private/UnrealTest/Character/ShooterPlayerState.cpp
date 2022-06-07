// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Character/ShooterPlayerState.h"

#include <UnrealTest/Game/ShooterGameState.h>

#include "Net/UnrealNetwork.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"

AShooterPlayerState::AShooterPlayerState()
{
	TeamNumber = 0;
}

void AShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterPlayerState, TeamNumber);
}

void AShooterPlayerState::OnRep_TeamNumberChanged()
{
	UpdateTeamColors();
}

void AShooterPlayerState::SetTeamNum(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
	UpdateTeamColors();
}

int32 AShooterPlayerState::GetTeamNum() const
{
	return TeamNumber;
}

void AShooterPlayerState::UpdateTeamColors() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (AUnrealTestCharacter* PlayerCharacter = Cast<AUnrealTestCharacter>(OwnerController->GetCharacter()))
		{
			PlayerCharacter->UpdateTeamColors(TeamNumber);
		}
	}
}
