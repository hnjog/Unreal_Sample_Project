#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SampleHealExecution.generated.h"

/*
	EGameplayEffectDurationType
	- Instant, Infinite, HasDuration
	-> 즉각, 영구, 기간
	(데미지,패시브, 버프/디버프 등에 대한 다양한 처리에 대한 지원)

	G Component 등으로(5.4 이상)
	세부 적인 처리가 더 가능하다고 하는데...
*/

UCLASS()
class SAMPLES_API USampleHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	USampleHealExecution();

	// UGameplayEffectExecutionCalculation의 Execute() BlueprintNativeEvent를 override
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
