// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Components/HealthComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
	: MaxHealth(100.f)
	, bInvincible(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	//Replicated by default in case the owner doesnt replicates it
	SetIsReplicated(true);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	ComponentOwner = GetOwner();
	UE_LOG(LogTemp, Warning, TEXT("UHealthComponent::BeginPlay"));

	if (GetOwnerRole() == ROLE_Authority && ComponentOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHealthComponent::BeginPlay Implementation"));
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::HandlePointDamage);
	}
}

void UHealthComponent::HandlePointDamage_Implementation(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("HandlePointDamage_Implementation"));
	if (bInvincible || Damage <= 0.f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, BoneName, InstigatedBy, DamageCauser);
}

bool UHealthComponent::HandlePointDamage_Validate(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	return true;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}
