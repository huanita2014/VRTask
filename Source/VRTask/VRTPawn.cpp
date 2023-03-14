// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTPawn.h"

#include "MotionControllerComponent.h"
#include "VRTaskBlueprintFunctionLibrary.h"
#include "VRTGameModeBase.h"
#include "VRTPickupActor.h"
#include "Components/VRTHealthComponent.h"
#include "Components/VRTPlayerHUDComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/VRTGrabComponent.h"
#include "Inventory/VRTInventoryComponent.h"

AVRTPawn::AVRTPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	HealthComponent = CreateDefaultSubobject<UVRTHealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UVRTInventoryComponent>(TEXT("InventoryComponent"));
	HUDComponent = CreateDefaultSubobject<UVRTPlayerHUDComponent>(TEXT("HUD Component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	BackpackComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Backpack"));
	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	MotionControllerLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftControllerCollision"));
	MotionControllerRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightControllerCollision"));

	RootComponent = DefaultRootComponent;
	CameraComponent->SetupAttachment(GetRootComponent());
	MotionControllerLeft->SetupAttachment(GetRootComponent());
	MotionControllerRight->SetupAttachment(GetRootComponent());
	MotionControllerLeftCollision->SetupAttachment(MotionControllerLeft);
	MotionControllerRightCollision->SetupAttachment(MotionControllerRight);
	BackpackComponent->SetupAttachment(CameraComponent);
}

void AVRTPawn::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AVRTPawn::TakeDamage);

	if(HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
	}

	if (BackpackComponent)
	{
		BackpackComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBackPackBeginOverlap);
		BackpackComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBackPackEndOverlap);
	}
}

void AVRTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("MenuNavigationLeftStickRight"), EInputEvent::IE_Pressed, this, &AVRTPawn::TryNavigateHUDRightWithLeftHand);
	PlayerInputComponent->BindAction(FName("MenuNavigationLeftStickLeft"), EInputEvent::IE_Pressed, this, &AVRTPawn::TryNavigateHUDLeftWithLeftHand);

}

void AVRTPawn::TryNavigateHUDLeftWithLeftHand()
{
	if (!HUDComponent)
	{
		return;
	}

	HUDComponent->HUDNavigateLeft(false);
}

void AVRTPawn::TryNavigateHUDRightWithLeftHand()
{
	if (!HUDComponent)
	{
		return;
	}

	HUDComponent->HUDNavigateRight(false);
}

void AVRTPawn::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	checkf(HealthComponent, TEXT("Attempted to take damage while HealthComponent is nullptr"));

	HealthComponent->ApplyDamage(Damage);
}

void AVRTPawn::HandleDeath(AActor* DeadActor)
{
	Destroy();
	
	if (const UWorld* World = GetWorld())
	{
		if (AVRTGameModeBase* GameMode = Cast<AVRTGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(GetController());
		}
	}
}

void AVRTPawn::OnBackPackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent == MotionControllerLeftCollision)
	{
		UVRTaskBlueprintFunctionLibrary::PlayHapticEffect(this, BackpackHapticEffect, EControllerHand::Left, 1.f, false);

		HeldComponentNearBackpack = HeldComponentLeft;
	}

	if (OverlappedComponent == MotionControllerRightCollision)
	{
		UVRTaskBlueprintFunctionLibrary::PlayHapticEffect(this, BackpackHapticEffect, EControllerHand::Right, 1.f, false);

		HeldComponentNearBackpack = HeldComponentRight;
	}
}

void AVRTPawn::OnBackPackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HeldComponentNearBackpack = nullptr;
}

void AVRTPawn::TryAddHeldPickupToInventory(UVRTGrabComponent* HeldComponent)
{
	if (!HeldComponent)
	{
		return;
	}
	
	AVRTPickupActor* HeldPickup = Cast<AVRTPickupActor>(HeldComponent->GetOwner());

	if (!HeldPickup)
	{
		return;
	}
			
	check(InventoryComponent);
	if (InventoryComponent->AddToInventory(HeldPickup))
	{
		if (HeldComponentLeft == HeldComponent)
		{
			HeldComponentLeft = nullptr;
		}

		if (HeldComponentRight == HeldComponent)
		{
			HeldComponentRight = nullptr;
		}
	}
}
