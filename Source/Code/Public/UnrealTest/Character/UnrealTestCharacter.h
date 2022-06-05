// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealTest/Components/HealthComponent.h"
#include "UnrealTest/Items/Weapons/WeaponBase.h"
#include "UnrealTestCharacter.generated.h"

UCLASS(config=Game)
class AUnrealTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUnrealTestCharacter();

	virtual void BeginPlay() override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE AWeaponBase* GetWeapon() const { return Weapon; }
	
	FORCEINLINE int32 GetCarriedAmmo() const { return CarriedAmmo; }
	FORCEINLINE void SetCarriedAmmo(int32 Ammo) { CarriedAmmo = Ammo; }

	FORCEINLINE AWeaponBase* GetEquippedWeapon() const { return Weapon; }
	
	void DisableCotrollerRotation();
	void ConfigureCharacterMovement(class UCharacterMovementComponent* characterMovement);
	void SetCameraBoom();
	void SetFollowCamera();
	
	const float JUMP_Z_VELOCITY= 700.f;
	const float AIR_CONTROL = 0.35f;
	const float MAX_WALK_SPEED = 500.f;
	const float MIN_ANALOG_WALK_SPEED = 20.f;
	const float BRAKING_DECELERATION_WALKING = 2000.f;

protected:
	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, FName BoneName, AController* InstigatedBy, AActor* DamageCauser);

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeaponBase> Weapon_BP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeaponBase> Weapon;
	
	/** Ammount of ammo charecter carries. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 CarriedAmmo;
};

