#include "SampleAnimInstance.h"
#include "AbilitySystemGlobals.h"

void USampleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}

	}
}

void USampleAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC 내부의 GameplayTag와 AnimInstance의 멤버 Property와 Delegate를 연결하여
	// 값 변화에 대한 반영 진행
	GameplayTagPropertyMap.Initialize(this, ASC);
}