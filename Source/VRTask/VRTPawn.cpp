// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTPawn.h"
#include "Components/VRTHealthComponent.h"

AVRTPawn::AVRTPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UVRTHealthComponent>(TEXT("HealthComponent"));
}

void AVRTPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVRTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

