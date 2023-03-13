// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTGameModeBase.h"
#include "VRTPawn.h"
#include "Components/VRTHealthComponent.h"

void AVRTGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AVRTPawn* PlayerPawn = Cast<AVRTPawn>(NewPlayer->GetPawn());
	UVRTHealthComponent* PlayerHealthComponent = PlayerPawn->GetHealthComponent();

	if (!PlayerHealthComponent)
	{
		return;
	}

	UE_LOG(LogGameMode, Display, TEXT("Player logged in"));

	PlayerHealthComponent->OnDeath.AddDynamic(this, &AVRTGameModeBase::HandlePlayerDeath);
}

void AVRTGameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AVRTGameModeBase::HandlePlayerDeath(AActor* DeadPlayer)
{
	ensureMsgf(DeadPlayer, TEXT("HandlePlayerDeath Failed since player is nullptr"));
	
	if (!DeadPlayer)
	{
		return;
	}

	UE_LOG(LogGameMode, Display, TEXT("Player '%s' died"), *DeadPlayer->GetName());
	RestartPlayer(Cast<APawn>(DeadPlayer)->GetController());
}
