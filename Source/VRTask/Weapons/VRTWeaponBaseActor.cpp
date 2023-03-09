// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTWeaponBaseActor.h"

DECLARE_LOG_CATEGORY_CLASS(LogWeapon, All, All);

// Sets default values
AVRTWeaponBaseActor::AVRTWeaponBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AVRTWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVRTWeaponBaseActor::TriggerPressed_Implementation()
{
	IVRTInteractableInterface::TriggerPressed_Implementation();

	UE_LOG(LogTemp, Warning, TEXT("TRIGGER PRESSED"));
}

// Called every frame
void AVRTWeaponBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

