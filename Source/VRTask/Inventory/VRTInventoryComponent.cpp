// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTInventoryComponent.h"

#include "VRTPickupsSubsystem.h"
#include "VRTask/VRTPickupActor.h"

UVRTInventoryComponent::UVRTInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UVRTInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PickupsSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UVRTPickupsSubsystem>();
}

TArray<FInventorySlot> UVRTInventoryComponent::GetInventory()
{
	return Inventory;
}

bool UVRTInventoryComponent::AddToInventory(AVRTPickupActor* PickupToAdd)
{
	if (!PickupToAdd && Inventory.Num() >= MaxSlots)
	{
		return false;
	}

	const FName& PickupID = PickupToAdd->GetPickupID();
	FPickupGameData PickupGameData;

	if (!GetPickupGameData(PickupID, PickupGameData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add pickup with ID: '%s' to the inventory!"), *PickupID.ToString());
		return false;
	}

	// Checking if pickup we are trying to add already exists in the inventory
	for (FInventorySlot& InventorySlot : Inventory)
	{
		if (InventorySlot.ItemID == PickupID)
		{
			if (PickupGameData.bIsStackable)
			{
				break;
			}
			
			InventorySlot.ItemCount++;
			return true;
		}
	}

	// Didn't found existing item with specified ID or pickup is not stackable, so adding a new slot
	FInventorySlot NewSlot;
	NewSlot.ItemID = PickupID;
	Inventory.Emplace(NewSlot);

	// Removing actor from the world
	PickupToAdd->Destroy();

	if (OnItemAdded.IsBound())
	{
		OnItemAdded.Broadcast(PickupID);
	}
	
	return true;
}

bool UVRTInventoryComponent::GetPickupGameData(const FName& PickupID, FPickupGameData& OutPickupGameData) const
{
	checkf(PickupsSubsystem, TEXT("Attempted to GetPickupGameData while Pickups subsystem is nullptr"));

	if (!PickupsSubsystem->FindPickupById(PickupID, OutPickupGameData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add pickup with ID: '%s' to the inventory!"), *PickupID.ToString());
		return false;
	}

	return true;
}

AVRTPickupActor* UVRTInventoryComponent::TakePickupFromInventory(const FName& PickupID)
{
	FInventorySlot InventorySlot;
	if (!GetInventorySlot(PickupID, InventorySlot))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to take pickup with specified id: '%s' from the inventory!"), *PickupID.ToString());
		return nullptr;
	}

	if (!RemoveItemFromInventory(PickupID))
	{
		return nullptr;
	}

	FPickupGameData PickupGameData;

	if (!GetPickupGameData(PickupID, PickupGameData))
	{
		return nullptr;
	}
	

	// Spawning pickup in the world
	const FActorSpawnParameters SpawnParams;
	const TSubclassOf<AVRTPickupActor> ClassToSpawn = PickupGameData.PickupActorClass;

	AVRTPickupActor* SpawnedActor = Cast<AVRTPickupActor>(GetWorld()->SpawnActor(*ClassToSpawn, nullptr, nullptr, SpawnParams));

	if (OnItemRemoved.IsBound())
	{
		OnItemRemoved.Broadcast(PickupID, SpawnedActor);
	}
	
	return SpawnedActor;
}


bool UVRTInventoryComponent::GetInventorySlot(const FName& ItemID, FInventorySlot& OutInventorySlot) const
{
	for (const FInventorySlot& InventorySlot : Inventory)
	{
		if (InventorySlot.ItemID == ItemID)
		{
			OutInventorySlot = InventorySlot;
			return true;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to find pickup with specified id: '%s' in the inventory!"), *ItemID.ToString());
	return false;
}

bool UVRTInventoryComponent::RemoveItemFromInventory(const FName& ItemID)
{
	FInventorySlot Slot;
	if (!GetInventorySlot(ItemID, Slot))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to remove item with specified id: '%s' from the inventory!"), *ItemID.ToString());
	}

	FPickupGameData PickupGameData;

	if (!GetPickupGameData(ItemID, PickupGameData))
	{
		return false;
	}

	if (PickupGameData.bIsStackable && Slot.ItemCount > 1)
	{
		Slot.ItemCount --;
		return true;
	}

	Inventory.RemoveSingle(Slot);
	return true;
}
