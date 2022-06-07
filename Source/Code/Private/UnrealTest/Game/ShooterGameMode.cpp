// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Game/ShooterGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "UnrealTest/Character/ShooterSpectatorPawn.h"
#include "UnrealTest/Game/ShooterGameState.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Controllers/LocalPlayerController.h"
#include "UnrealTest/Controllers/LocalPlayerStart.h"

AShooterGameMode::AShooterGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerPawnObject(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	DefaultPawnClass = PlayerPawnObject.Class;
	
	PlayerStateClass = AShooterPlayerState::StaticClass();
	GameStateClass = AShooterGameState::StaticClass();
	PlayerControllerClass = ALocalPlayerController::StaticClass();
	SpectatorClass = AShooterSpectatorPawn::StaticClass();

	NumTeams = 2;
}


AActor* AShooterGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AShooterPlayerState* NewPlayerState = Cast<AShooterPlayerState>(Player->PlayerState);
	const int32 TeamNum = ChooseTeam(NewPlayerState);
	NewPlayerState->SetTeamNum(TeamNum);
	
	TArray<APlayerStart*> PreferredSpawns;
	APlayerStart* BestStart = nullptr;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* start = *It;
		if (start->IsA<ALocalPlayerStart>())
		{
			if (CheckStartTeam(start, Player))
			{
				PreferredSpawns.Add(start);
			}
		}
	}

	if (!BestStart)
	{
		if (PreferredSpawns.Num() > 0)
		{
			BestStart = PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];
		}
	}
	
	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

bool AShooterGameMode::CheckStartTeam(APlayerStart* PlayerStart, AController* Player) const
{
	if (Player)
	{
		ALocalPlayerStart* CustomStart = Cast<ALocalPlayerStart>(PlayerStart);
		AShooterPlayerState* ShooterPlayerState = Cast<AShooterPlayerState>(Player->PlayerState);

		if (ShooterPlayerState && CustomStart && CustomStart->GetSpawnTeam() == ShooterPlayerState->GetTeamNum())
		{
			return true;
		}
	}
	return false;
}

void AShooterGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NumPlayers == 4)
	{
		OnMatchStart.Broadcast();
		StartMatch();
	}
}

int32 AShooterGameMode::ChooseTeam(AShooterPlayerState* PlayerState) const
{
	int32 PlayersTeam0 = 0;
	int32 PlayersTeam1 = 0;

	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AShooterPlayerState* ShooterPlayerState = Cast<AShooterPlayerState>(GameState->PlayerArray[i]);
		if (ShooterPlayerState)
		{
			ShooterPlayerState->GetTeamNum() == 0 ? PlayersTeam0++ : PlayersTeam1++;
		}
	}
	return PlayersTeam0 <= PlayersTeam1 ? 0 : 1;
}