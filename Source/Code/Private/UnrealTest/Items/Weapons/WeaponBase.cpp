// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTest/Items/Weapons/WeaponBase.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealTest/Character/UnrealTestCharacter.h"
#include "UnrealTest/Controllers/LocalPlayerController.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetupAttachment(Root);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(Root);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponOwner = Cast<AUnrealTestCharacter>(GetOwner());
	check(WeaponOwner);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TPair<FVector, FVector> AWeaponBase::GetCameraTrace() const
{
	if (!ItemMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("ARangeWeaponBase::WeaponTrace: Weapon mesh is null!"));
		return {};
	}

	ALocalPlayerController* PlayerController = Cast<ALocalPlayerController>(WeaponOwner->GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("ARangeWeaponBase::WeaponTrace: Player controller is null!"));
		return {};
	}

	const APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ARangeWeaponBase::WeaponTrace: Camera manager is null!"));
		return {};
	}

	const FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraManager->GetCameraRotation());
	const FVector CameraLocation = CameraManager->GetCameraLocation();

	const FVector StartTrace = CameraLocation;
	// Mesh->GetSocketLocation(Constants::Sockets::Weapons::WEAPON_MUZZLE_FLASH_POINT_NAME);
	const FVector EndTrace = CameraLocation + CameraForwardVector * /*WeaponMetaData.FireRange*/ 5000;

	return TPair<FVector, FVector>(StartTrace, EndTrace);
}

FVector AWeaponBase::GetCameraTraceHitLocation()
{
	const TPair<FVector, FVector> CameraTrace = GetCameraTrace();

	FHitResult HitResult(ForceInit);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
	                                      CameraTrace.Key,
	                                      CameraTrace.Value,
	                                      UEngineTypes::ConvertToTraceType(ECC_Visibility),
	                                      true,
	                                      {this, GetOwner()},
	                                      /*FireTraceDebugData.DrawDebugTraceType*/EDrawDebugTrace::None,
	                                      HitResult,
	                                      true,
	                                      /*FireTraceDebugData.TraceColor*/ FLinearColor::Red,
	                                      /*FireTraceDebugData.TraceHitColor*/ FLinearColor::Blue,
	                                      /*FireTraceDebugData.DrawTime*/ 2.f);

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

void AWeaponBase::Fire()
{
	TPair<FVector, FVector> Trace = GetWeaponTrace();

	const FVector StartTrace = Trace.Key;
	const FVector EndTrace = Trace.Value;

	FHitResult HitResult(ForceInit);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
	                                      StartTrace,
	                                      EndTrace,
	                                      UEngineTypes::ConvertToTraceType(ECC_Visibility),
	                                      true,
	                                      {this, GetOwner()},
	                                      /*FireTraceDebugData.DrawDebugTraceType*/EDrawDebugTrace::ForDuration,
	                                      HitResult,
	                                      true,
	                                      /*FireTraceDebugData.TraceColor*/ FLinearColor::Green,
	                                      /*FireTraceDebugData.TraceHitColor*/ FLinearColor::Yellow,
	                                      /*FireTraceDebugData.DrawTime*/ 2.f);

	if (!HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Log, TEXT("ARangeWeaponBase::FireTrace: Weapon trace hit nothing!"));
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ARangeWeaponBase::FireTrace: Didn't hit any actor!"));
		return;
	}

	UGameplayStatics::ApplyPointDamage(HitActor,
	                                   /*WeaponMetaData.HitDamage*/ 10,
	                                   StartTrace,
	                                   HitResult,
	                                   GetOwner()->GetInstigatorController(),
	                                   this,
	                                   /*WeaponMetaData.DamageType*/ {});
}

void AWeaponBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
}

void AWeaponBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
