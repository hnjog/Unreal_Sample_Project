#include "SampleAbilitySet.h"
#include "Samples/AbilitySystem/Abilities/SampleGameplayAbility.h"
#include "Samples/AbilitySystem/SampleAbilitySystemComponent.h"

void FSampleAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
		AbilitySpecHandles.Add(Handle);
}

void FSampleAbilitySet_GrantedHandles::TakeFromAbilitySystem(USampleAbilitySystemComponent* SampleASC)
{
}

USampleAbilitySet::USampleAbilitySet(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void USampleAbilitySet::GiveToAbilitySystem(USampleAbilitySystemComponent* SampleASC, FSampleAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(SampleASC);

	if (!SampleASC->IsOwnerActorAuthoritative())
		return;

	// 전부 순회
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FSampleAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		// GiveAbility()에서 만약 EGameplayAbilityInstancingPolicy::InstancedPerActor라면,
		// 내부적으로 Instance를 생성하지만 그렇지 않으면 CDO를 사용
		// 
		// EGameplayAbilityInstancingPolicy
		// - NotInstance, (생성 안함 - CDO 형태로만 들고 있을 수 있음)
		// InstancedPerActor, (Actor 별로만 만든다 -> 보통 많이 사용)
		// InstancedPerExecution (사용할때마다 만든다 -> 너무 무거움)
		// 3가지 Type 존재(위로 갈수록 이득)
		// CDO로만 사용할순?
		// -> 무조건 기본값 들고옴
		// 
		// USampleGameplayAbility는 UObject를 상속받았기에 GC의 대상,
		// UObject가 점점 늘어나게 되는것은 성능 및 메모리에 좋지 않음
		// 
		// - 이를 통해 UObject 갯수를 줄여 UObject가 늘어남에 따라 
		// 늘어나는 성능적/메모리적 부하를 줄일 수 있음
		// 
		// Lyra 같은 FPS의 경우는 RPG와는 다르게 스킬이나 무기별 데미지가 달라질 일이 없으므로
		// CDO를 사용해도 괜찮은?
		//
		USampleGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<USampleGameplayAbility>();

		// AbilitySpec은 GiveAbility로 전달되어, ActivatbleAbilities에 추가
		// FGameplayAbilitySpec 를 통해 래핑
		// (DynamicAbilityTags 를 통해 태그 매핑 가능)
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		// GiveAbility를 통해 Spec를 집어넣고
		// 내부에서 Container에서 보관 -> Handle값으로 관리
		// 그 Handle을 보관함으로서 접근을 매우 빠르게 할 수 있음
		// (Handle을 Index값으로 사용)
		const FGameplayAbilitySpecHandle AbilitySpecHandle = SampleASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}
