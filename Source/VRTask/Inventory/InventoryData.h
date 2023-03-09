﻿#pragma once

#include "CoreMinimal.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	bool bIsStackable;
};