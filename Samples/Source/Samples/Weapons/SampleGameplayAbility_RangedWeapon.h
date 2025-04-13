#pragma once

#include "CoreMinimal.h"
#include "Samples/Equipment/SampleGameplayAbility_FromEquipment.h"
#include "SampleGameplayAbility_RangedWeapon.generated.h"

UCLASS()
class SAMPLES_API USampleGameplayAbility_RangedWeapon : public USampleGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	USampleGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
