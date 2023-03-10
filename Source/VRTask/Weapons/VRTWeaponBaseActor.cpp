// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTWeaponBaseActor.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "VRTWeaponAmmoWidget.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_CLASS(LogWeapon, All, All);

AVRTWeaponBaseActor::AVRTWeaponBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMeshComponent");
	RootComponent = WeaponMeshComponent;

	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("AmmoWidgetComponent");
	AmmoWidgetComponent->SetupAttachment(RootComponent);
	
	
}

void AVRTWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentFiringMode = EFiringMode::Auto;
	
	Ammo = MaxAmmo;

	WeaponAmmoWidget = Cast<UVRTWeaponAmmoWidget>(AmmoWidgetComponent->GetWidget());

	if (WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateAmmo(Ammo);
	}
}

void AVRTWeaponBaseActor::TriggerPressed_Implementation()
{
	IVRTInteractableInterface::TriggerPressed_Implementation();

	StartFire();
}

void AVRTWeaponBaseActor::DrawTraceLine()
{
	const FTransform MuzzleTransform = GetMuzzleTransform();

	const FVector TraceStart = MuzzleTransform.GetLocation();
	const FVector ShootingDirection = MuzzleTransform.GetRotation().Vector();
	const FVector TraceEnd = TraceStart + ShootingDirection * TraceMaxDistance;

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Emerald, false, 0.1f);
}

void AVRTWeaponBaseActor::StartFire()
{
	if (Ammo == 0)
	{
		return;
	}

	switch (CurrentFiringMode)
	{
		case EFiringMode::Manual:
			Fire();
			break;
		case EFiringMode::Auto:
			//const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
			GetWorldTimerManager().SetTimer(TimeBetweenShotsHandler, this, &AVRTWeaponBaseActor::Fire, TimeBetweenShots, true);
			break;	
	}
}

void AVRTWeaponBaseActor::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimeBetweenShotsHandler);
}

void AVRTWeaponBaseActor::Reload()
{
	Ammo = MaxAmmo;
}

void AVRTWeaponBaseActor::PlayFireEffect()
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
}

void AVRTWeaponBaseActor::PlayImpactEffect(FVector EffectLocation)
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, EffectLocation);
	}
}

const FTransform AVRTWeaponBaseActor::GetMuzzleTransform() const
{
	return WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
}

APlayerController* AVRTWeaponBaseActor::GetPlayerController() const
{
	const APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (PlayerPawn)
	{
		return Cast<APlayerController>(PlayerPawn->GetController());
	}
	return nullptr;
}

void AVRTWeaponBaseActor::PlayHapticEffect(UHapticFeedbackEffect_Base* InHapticEffect) const
{
	APlayerController* PlayerController = GetPlayerController();

	if (PlayerController)
	{
		PlayerController->PlayHapticEffect(InHapticEffect, CurrentHand, 1.f, false);
	}
}

void AVRTWeaponBaseActor::Fire()
{
	if (Ammo == 0)
	{
		EndFire();
		
		if (EmptyFireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
		}

		return;
	}

	Ammo -=  1;

	if (WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateAmmo(Ammo);
	}
	
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}

	PlayHapticEffect(FireHapticEffect);
}

void AVRTWeaponBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawTraceLine();

}

