// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "LocalPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API ALocalPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	ALocalPlayerStart(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE int32 GetSpawnTeam() const { return SpawnTeam; }
	
private:
	UPROPERTY(EditAnywhere, Category = "Team", meta = (AllowPrivateAccess = "true"))
	int32 SpawnTeam;
};
