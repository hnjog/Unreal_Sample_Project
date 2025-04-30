#pragma once

#include "CoreMinimal.h"
#include "SampleGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "SampleAbilityCost.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class SAMPLES_API USampleAbilityCost : public UObject
{
	GENERATED_BODY()
public:
	USampleAbilityCost();

	virtual bool CheckCost(const USampleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	virtual void ApplyCost(const USampleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{

	}

};
