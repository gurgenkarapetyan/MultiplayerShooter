// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Controllers/LocalPlayerStart.h"

ALocalPlayerStart::ALocalPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("AUT_CustomPlayerStart::AUT_CustomPlayerStart"));
	SpawnTeam = -1;
}
