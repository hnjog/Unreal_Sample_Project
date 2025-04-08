#pragma once

#include "SampleInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "SampleInventoryFragment_EquippableItem.generated.h"

class USampleEquipmentDefinition;

UCLASS()
class USampleInventoryFragment_EquippableItem : public USampleInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category=Sample)
	TSubclassOf<USampleEquipmentDefinition> EquipmentDefinition;
};