// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Items/Weapons/WeaponBase.h"

#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Controllers/LocalPlayerController.h"

AWeaponBase::AWeaponBase()
	: bShouldFire(false)
	, AutomaticFireRate(0.1f)
	, ShootTimeDuration(0.05f)
	, Ammo(30)
	, MagazineCapacity(30)
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetupAttachment(Root);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(Root);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponOwner = Cast<AUnrealTestCharacter>(GetOwner());
	check(WeaponOwner);
}

void AWeaponBase::StartFire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerStartFire();
	}
	
	if (!bShouldFire)
	{
		bShouldFire = true;
		Fire();
	}
}

void AWeaponBase::ServerStartFire_Implementation()
{
	StartFire();
}

bool AWeaponBase::ServerStartFire_Validate()
{
	return true;
}

void AWeaponBase::StopFire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerStopFire();
	}
	
	if (bShouldFire)
	{
		bShouldFire = false;
		GetWorldTimerManager().ClearTimer(AutoFireTimer);
	}
}

void AWeaponBase::ServerStopFire_Implementation()
{
	StopFire();
}

bool AWeaponBase::ServerStopFire_Validate()
{
	return true;
}

void AWeaponBase::Fire()
{
	GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AWeaponBase::AutoFireReset, AutomaticFireRate);
}

void AWeaponBase::AutoFireReset()
{
	if (WeaponHasAmmo())
	{
		StartFiring();
	}
	else
	{
		Reload();
	}
}

bool AWeaponBase::WeaponHasAmmo()
{
	return Ammo > 0;
}

void AWeaponBase::StartFiring()
{
	SetFireLineTrace();
	PlayFireSoundCue();
	DecrementAmmo();
	Fire();
}

void AWeaponBase::SetFireLineTrace()
{
	TPair<FVector, FVector> Trace = GetWeaponTrace();

	const FVector StartTrace = Trace.Key;
	const FVector EndTrace = Trace.Value;

	FHitResult HitResult(ForceInit);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, {this, GetOwner()}, EDrawDebugTrace::ForDuration,HitResult, true,FLinearColor::Green,FLinearColor::Yellow,2.f);

	if (!HitResult.bBlockingHit)
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		return;
	}

	UGameplayStatics::ApplyPointDamage(HitActor, 10.f, StartTrace, HitResult, GetOwner()->GetInstigatorController(), this, {});
}

TPair<FVector, FVector> AWeaponBase::GetCameraTrace() const
{
	if (!ItemMesh)
	{
		return {};
	}

	ALocalPlayerController* PlayerController = Cast<ALocalPlayerController>(WeaponOwner->GetController());
	if (!PlayerController)
	{
		return {};
	}

	const APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager)
	{
		return {};
	}

	const FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraManager->GetCameraRotation());
	const FVector CameraLocation = CameraManager->GetCameraLocation();

	const FVector StartTrace = CameraLocation;
	const FVector EndTrace = CameraLocation + CameraForwardVector * 5000;

	return TPair<FVector, FVector>(StartTrace, EndTrace);
}

FVector AWeaponBase::GetCameraTraceHitLocation()
{
	const TPair<FVector, FVector> CameraTrace = GetCameraTrace();

	FHitResult HitResult(ForceInit);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), CameraTrace.Key, CameraTrace.Value, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, {this, GetOwner()}, EDrawDebugTrace::None, HitResult, true,FLinearColor::Red, FLinearColor::Blue, 2.f);

	if (!HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Log, TEXT("ARangeWeaponBase::Fire: Trace hit nothing!"));
		return {};
	}

	return HitResult.Location;
}

TPair<FVector, FVector> AWeaponBase::GetWeaponTrace()
{
	check(ItemMesh);

	const FVector CameraHitLocation = GetCameraTraceHitLocation();

	const FVector MuzzleLocation = ItemMesh->GetSocketLocation("Muzzle");
	const FVector WeaponTraceDir = UKismetMathLibrary::GetDirectionUnitVector(MuzzleLocation, CameraHitLocation);
	const FVector WeaponTraceEndLocation = CameraHitLocation + WeaponTraceDir * /*WeaponMetaData.FireRange*/ 6000;

	return {MuzzleLocation, WeaponTraceEndLocation};
}

void AWeaponBase::PlayFireSoundCue()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}	
}

void AWeaponBase::DecrementAmmo()
{
	--Ammo;
	if (Ammo <= 0)
	{
		Ammo = 0;
	}
}

void AWeaponBase::Reload()
{
	if (ClipIsFull())
	{
		return;
	}

	int32 CarriedAmmo = WeaponOwner->GetCarriedAmmo(); 

	// Space left in the magazine of EquippedWeapon
	const int32 MagazineEmptySpace = MagazineCapacity - Ammo;

	if (MagazineEmptySpace > CarriedAmmo)
	{
		// Reload the magazine with all the ammo we are carrying
		ReloadAmmo(CarriedAmmo);
		WeaponOwner->SetCarriedAmmo(CarriedAmmo);
	}
	else
	{
		// Fill the magazine
		ReloadAmmo(MagazineEmptySpace);
		CarriedAmmo -= MagazineEmptySpace;
		WeaponOwner->SetCarriedAmmo(CarriedAmmo);
	}
}

void AWeaponBase::ReloadAmmo(int32 Amount)
{
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("Attempted to reload with more than magazine capacity"));
	Ammo += Amount;
}

bool AWeaponBase::ClipIsFull() const
{
	return Ammo >= MagazineCapacity;
}
