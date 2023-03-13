// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRTPlayerHUDComponent.generated.h"

class AVRTWorldWidgetActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNavigateLeftDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNavigateRightDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerPressedDelegate, bool, bRightHand);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTASK_API UVRTPlayerHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVRTPlayerHUDComponent();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleMenuWidget(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleInventoryWidget(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseActiveWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HUDNavigateLeft(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HUDNavigateRight(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HUDTriggerDown(bool bRightHand);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "UI")
	TSubclassOf<AVRTWorldWidgetActor> MainMenuWidgetActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "UI")
	TSubclassOf<AVRTWorldWidgetActor> InventoryWidgetActorClass;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FNavigateLeftDelegate OnNavigateLeft;
	
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FNavigateRightDelegate OnNavigateRight;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FTriggerPressedDelegate OnTriggerPressed;

private:

	UPROPERTY()
	AVRTWorldWidgetActor* ActiveWidget;

	void SpawnWidget(TSubclassOf<AVRTWorldWidgetActor> WidgetToSpawn, bool bRightHand);

	bool bActiveHandRight;
	
};
