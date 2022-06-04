// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
	: MaxHealth(100.f)
	, bInvincible(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::HandlePointDamage);
	}
}

void UHealthComponent::HandlePointDamage(AActor* DamagedActor, float Damage,AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (bInvincible || Damage <= 0.f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, BoneName, InstigatedBy, DamageCauser);
}
