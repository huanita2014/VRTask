// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTGrabComponent.h"

UVRTGrabComponent::UVRTGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UVRTGrabComponent::TryGrab()
{
	SetPrimitiveComponentPhysics(false);
	AttachParentToMotionController(MotionController);
	bIsHeld = true;

	if (GrabType == EGrabbingType::Snap)
	{
		// Orient the held Actor to match GrabComponent's relative location
		GetAttachParent()->SetRelativeRotation(GetRelativeRotation().GetInverse(), false, nullptr, ETeleportType::TeleportPhysics);
	}
}


void UVRTGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UVRTGrabComponent::SetPrimitiveComponentPhysics(bool bSimulate) const
{
	USceneComponent* AttachedParent = GetAttachParent();

	if (!AttachedParent)
	{
		return;
	}

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(AttachedParent);

	if (!PrimitiveComponent)
	{
		return;
	}

	PrimitiveComponent->SetSimulatePhysics(bSimulate);
}

void UVRTGrabComponent::AttachParentToMotionController(UMotionControllerComponent* InMotionController)
{
	if (!MotionController)
	{
		return;
	}

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, true);
	
	
	//if (!InMotionController->AttachToComponent(GetAttachParent(), AttachmentTransformRules))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attach failed"));
	}
}
