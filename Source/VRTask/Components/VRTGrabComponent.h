// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VRTGrabComponent.generated.h"

/*
 * Basically the C++ clone of Unreal VR template GrabComponent with some unused logic cut
 */

UENUM(BlueprintType)
enum class EGrabbingType : uint8
{
	Free,
	Snap
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTASK_API UVRTGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UVRTGrabComponent();

	
	void TryGrab();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class UMotionControllerComponent* MotionController;

private:
	bool bIsHeld;

	EGrabbingType GrabType;

	void SetPrimitiveComponentPhysics(bool bSimulate) const;

	void AttachParentToMotionController(UMotionControllerComponent* InMotionController);
};
