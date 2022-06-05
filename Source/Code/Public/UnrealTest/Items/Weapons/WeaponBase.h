// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AUnrealTestCharacter;
class UBoxComponent;
class UWidgetComponent;
class USphereComponent;
class USoundBase;

UCLASS()
class UNREALTEST_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return  ItemMesh; }

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

	void StartFire();
	void StopFire();
	
	void Reload();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Called when overlapping AreaSphere. */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Called when end overlapping AreaSphere. */
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(reliable, server, WithValidation)
	void ServerStartFire();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopFire();
	
	TPair<FVector, FVector> GetCameraTrace() const;
	FVector GetCameraTraceHitLocation();
	TPair<FVector, FVector> GetWeaponTrace();

private:
    /** Playing fire sound cue when character starts firing. */
	void PlayFireSoundCue();

	/** Create particle effect when character starts firing. */
	void CreateFireMuzzleFlashParticle();

	bool WeaponHasAmmo();
	
	void AutoFireReset();
	void StartFiring();
	void SetFireLineTrace();
	void Fire();
	void DecrementAmmo();

	bool ClipIsFull() const;

	void ReloadAmmo(int32 Amount);
	
protected:
	UPROPERTY()
	TObjectPtr<AUnrealTestCharacter> WeaponOwner;
	
private:
	/** Scene component for a Root. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
	
	/** Skeleton mesh for the item. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> ItemMesh;
	
	/** Line trace collides with box to show HUD widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> CollisionBox;
	
	/** Randomized gunshot sound cue. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> FireSound;

	/** Flash spawned at barrel socket. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;
	
	/** True when we can fire. False waiting for the timer. */
	bool bShouldFire;
	
	/** Rate of automatic gun fire. */
	float AutomaticFireRate;

	/** Sets a timer between gunshots. */
	FTimerHandle AutoFireTimer;
	
	float ShootTimeDuration;

	/** Ammo count for this Weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	/** Maximum ammo that our weapon can hold. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;
};
