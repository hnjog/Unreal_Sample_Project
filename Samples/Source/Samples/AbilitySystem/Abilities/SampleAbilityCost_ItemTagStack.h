#pragma once

#include "CoreMinimal.h"
#include "SampleAbilityCost.h"
#include "SampleAbilityCost_ItemTagStack.generated.h"

UCLASS(meta = (DisplayName = "Item Tag Stack"))
class SAMPLES_API USampleAbilityCost_ItemTagStack : public USampleAbilityCost
{
	GENERATED_BODY()
public:
	USampleAbilityCost_ItemTagStack();

	virtual bool CheckCost(const USampleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const USampleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

public:
	// Cost를 확인하는 값
	// FScalableFloat : CurveData 
	// (특정 상황에 따라 다른값이 필요할 때 사용 ex : 스킬 레벨에 따른 마나소모값)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FScalableFloat Quantity;

	// 코스트용 Tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag Tag;

	// CheckCost 실패에 따른 이유를 알리는 Tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag FailureTag;

};
