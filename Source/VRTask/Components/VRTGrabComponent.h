// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VRTGrabComponent.generated.h"

/*
 * Basically the C++ clone of Unreal VR template GrabComponent with some unused logic cut
 */

class UMotionControllerComponent;
UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTASK_API UVRTGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UVRTGrabComponent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game")
	bool TryGrab(UMotionControllerComponent* InMotionController);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game")
	bool TryRelease();
};
