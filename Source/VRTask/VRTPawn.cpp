// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTPawn.h"
#include "VRTGameModeBase.h"
#include "Components/VRTHealthComponent.h"
#include "Components/VRTPlayerHUDComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Inventory/VRTInventoryComponent.h"

AVRTPawn::AVRTPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UVRTHealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UVRTInventoryComponent>(TEXT("InventoryComponent"));
	HUDComponent = CreateDefaultSubobject<UVRTPlayerHUDComponent>(TEXT("HUD Component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	BackpackComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Backpack"));

	CameraComponent->SetupAttachment(GetRootComponent());
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
	
	//Get the World and GameMode in the world to invoke its restart player function.
	if (const UWorld* World = GetWorld())
	{
		if (AVRTGameModeBase* GameMode = Cast<AVRTGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(GetController());
		}
	}
}

