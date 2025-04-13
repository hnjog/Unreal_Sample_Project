#include "SampleAbilitySystemComponent.h"
#include "Samples/AbilitySystem/Abilities/SampleGameplayAbility.h"

USampleAbilitySystemComponent::USampleAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USampleAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		// ActivatableAbilities : UAbilitySystemComponent 에 존재하는
		// '사용 가능한 Ability'를 포함하는 컨테이너
		// (게임적이라면 Level에 따라 사용 가능한 스킬을 포함하는 자료구조)
		// 
		// FGameplayAbilitySpec : GamePlayAbility의 Runtime에 필요한 데이터를
		// 따로 가지고 있는 객체
		// 
		// 허용되어 있는 AbilitySpec 순회
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// Ability가 존재하며
			// DynamicAbilityTags 의 InputTag에 있을 경우
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				// InputPressed/Held에 넣어 Ability 처리 대기
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void USampleAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				// Release에 추가, Held에서 제거
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}
