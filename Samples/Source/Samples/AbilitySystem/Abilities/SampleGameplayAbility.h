#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SampleGameplayAbility.generated.h"

UCLASS()
class SAMPLES_API USampleGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	USampleGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
