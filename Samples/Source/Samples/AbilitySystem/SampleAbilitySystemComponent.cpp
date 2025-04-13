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

void USampleAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// InputHeldSpecHandles에 대해 Ability 처리를 위해 AbilitiesToActivate 에 추가
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		// FindAbilitySpecFromHandle : ActivatableAbilities 내부에서 Handle 값 비교를 통해 FGameplayAbilitySpec 반환
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const USampleGameplayAbility* SampleAbilityCDO = CastChecked<USampleGameplayAbility>(AbilitySpec->Ability);

				// ActivationPolicy check
				// 누르는 동안 발동된다면 계속 넣어준다
				if (SampleAbilityCDO->ActivationPolicy == ESampleAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		// FindAbilitySpecFromHandle : ActivatableAbilities 내부에서 Handle 값 비교를 통해 FGameplayAbilitySpec 반환
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// 이미 활성화되어 있을 경우, InputEvent만 호출
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const USampleGameplayAbility* SampleAbilityCDO = CastChecked<USampleGameplayAbility>(AbilitySpec->Ability);
					if (SampleAbilityCDO->ActivationPolicy == ESampleAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// 등록된 AbilitiesToActivate를 한꺼번에 등록
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		// CallActivate 호출로 BP의 Activate 노드를 실행 시킨다
		TryActivateAbility(AbilitySpecHandle);
	}

	// 이번 프레임에 Release되었다면, 관련 GameplayAbility 처리
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// InputHeld는 InputReleasedSpecHandles가 추가될때 제거됨
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
