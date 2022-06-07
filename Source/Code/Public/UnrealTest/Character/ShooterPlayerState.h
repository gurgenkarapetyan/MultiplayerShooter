// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTEST_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AShooterPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE int32 GetTeamNumber() const { return TeamNumber; }
	
	UFUNCTION()
	void OnRep_TeamNumberChanged();
	
	/** Sets Team */
	void SetTeamNum(int32 NewTeamNumber);

	/** Updates Mesh Colors With the team id -- Red or Blue */
	void UpdateTeamColors() const;

	UFUNCTION(BlueprintCallable)
	int32 GetTeamNum() const;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_TeamNumberChanged, meta = (AllowPrivateAccess = "true"))
	int32 TeamNumber;
};
