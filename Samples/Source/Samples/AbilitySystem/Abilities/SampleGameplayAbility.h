#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SampleGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ESampleAbilityActivationPolicy :uint8
{
	OnInputTriggered, // Input이 Trigger되었을 경우 (pressed/released)
	WhileInputActive, // Input이 Held 된 경우
	OnSpawn, // Avatar가 생성되었을 경우, 바로 할당 (Passive 계열)
};

class USampleAbilityCost;

UCLASS()
class SAMPLES_API USampleGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	USampleGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

public:
	// 언제 GA가 활성화 될지의 정책
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sample|AbilityActivation")
	ESampleAbilityActivationPolicy ActivationPolicy;

	// 추가적인 어빌리티의 비용
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Sample|Costs")
	TArray<TObjectPtr<USampleAbilityCost>> AdditionalCosts;
};
