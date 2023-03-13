// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRTask//Interfaces/VRTInteractableInterface.h"
#include "VRTask/VRTPickupActor.h"
#include "VRTWeaponBaseActor.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UWidgetComponent;
class UVRTWeaponAmmoWidget;
class USoundBase;
class UHapticFeedbackEffect_Base;

UENUM(BlueprintType)
enum class EFiringMode : uint8
{
	Manual,
	Auto
};

UCLASS()
class VRTASK_API AVRTWeaponBaseActor : public AVRTPickupActor, public IVRTInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AVRTWeaponBaseActor();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* AmmoWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* MuzzleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TraceMaxDistance = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 MaxAmmo = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	float FireRate = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	float FireRange = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	USoundBase* EmptyFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	TSet<EFiringMode> AvailableFiringMods;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	UHapticFeedbackEffect_Base* FireHapticEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	TSubclassOf<UDamageType> DamageType;

	// VRTInteractableInterface
	void TriggerPressed_Implementation() override;

	void DrawTraceLine();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void EndFire();

	void Reload();
	void Fire();
	void PlayFireEffect();
	void PlayImpactEffect(FVector EffectLocation);

private:

	const FTransform GetMuzzleTransform() const;

	int32 Ammo;

	UPROPERTY()
	UVRTWeaponAmmoWidget* WeaponAmmoWidget;

	float TimeBetweenShots = 0.1f;
	float LastFireTime;
	FTimerHandle TimeBetweenShotsHandler;
	bool bCanShoot = true;
	bool bCanReload = true;

	EFiringMode CurrentFiringMode;
	EControllerHand CurrentHand = EControllerHand::Left;

	APlayerController* GetPlayerController() const;
	void PlayHapticEffect(UHapticFeedbackEffect_Base* InHapticEffect) const;

};
