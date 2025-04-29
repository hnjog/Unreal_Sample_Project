// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SampleInventoryItemDefinition.h"
#include "GameplayTagContainer.h"
#include "SampleInventoryFragment_SetStats.generated.h"

UCLASS()
class SAMPLES_API USampleInventoryFragment_SetStats : public USampleInventoryItemFragment
{
	GENERATED_BODY()
public:

	virtual void OnInstanceCreated(USampleInventoryItemInstance* Instance) const override;
public:
	// GameplayTagStackContainer를 통해
	// 주어질 아이템의 stat
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;
};
