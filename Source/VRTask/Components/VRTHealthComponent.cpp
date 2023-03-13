// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTHealthComponent.h"

#include "VRTask/VRTPawn.h"

UVRTHealthComponent::UVRTHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UVRTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (StartingHealth <= 0.0f)
	{
		StartingHealth = MaxHealth;
	}

	CurrentHealth = StartingHealth;

}


// Called every frame
void UVRTHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UVRTHealthComponent::GetCurrentHealthPercent()
{
	return (CurrentHealth / MaxHealth);
}

void UVRTHealthComponent::ApplyDamage(float Damage)
{
	if (bIsDead)
	{
		return;
	}
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, MinHealth, MaxHealth);

	if (CurrentHealth == MinHealth)
	{
		Death();
	}

	OnHealthUpdatedDelegate.Broadcast(CurrentHealth);
}

void UVRTHealthComponent::Death()
{
	bIsDead = true;

	if (OnDeath.IsBound())
	{
		OnDeath.Broadcast(GetOwner());
	}
}

