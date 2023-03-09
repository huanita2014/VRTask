// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/VRTInteractableInterface.h"
#include "VRTWeaponBaseActor.generated.h"

UCLASS()
class VRTASK_API AVRTWeaponBaseActor : public AActor, public IVRTInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRTWeaponBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// VRTInteractableInterface
	void TriggerPressed_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
