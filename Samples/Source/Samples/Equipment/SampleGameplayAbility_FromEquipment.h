﻿#pragma once

#include "CoreMinimal.h"
#include "Samples/AbilitySystem/Abilities/SampleGameplayAbility.h"
#include "SampleGameplayAbility_FromEquipment.generated.h"

class USampleEquipmentInstance;
class USampleInventoryItemInstance;

UCLASS()
class SAMPLES_API USampleGameplayAbility_FromEquipment : public USampleGameplayAbility
{
	GENERATED_BODY()
public:
	USampleGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USampleEquipmentInstance* GetAssociatedEquipment() const;
	USampleInventoryItemInstance* GetAssociatedItem() const;
};
