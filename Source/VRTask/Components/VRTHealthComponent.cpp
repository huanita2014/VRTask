// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTHealthComponent.h"

UVRTHealthComponent::UVRTHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UVRTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

}


// Called every frame
void UVRTHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

