// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class UNREALTEST_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return  ItemMesh; }
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Called when overlapping AreaSphere. */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Called when end overlapping AreaSphere. */
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/** Skeleton mesh for the item. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess="true"))
	USkeletalMeshComponent* ItemMesh;
	
	/** Line trace collides with box to show HUD widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess="true"))
	UBoxComponent* CollisionBox;
	

	/** Enable item tracing when overlapped. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess="true"))
	USphereComponent* AreaSphere;
};
