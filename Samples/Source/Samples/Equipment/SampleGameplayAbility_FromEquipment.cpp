#include "SampleGameplayAbility_FromEquipment.h"
#include "SampleEquipmentInstance.h"

USampleGameplayAbility_FromEquipment::USampleGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

USampleEquipmentInstance* USampleGameplayAbility_FromEquipment::GetAssociatedEquipment()
{
	// CurrentActorInfo는 ASC와 CurrentSpecHandle을 활용해, GameplayAbilitySpec를 가져옴
	// - CurrentSpecHandle은 SetCurrentActorInfo() 호출할 때, Handle 값을 받아 저장됨
	// - CurrentSpecHandle과 CurrentActorInfo 는 같이 함
	// - FindAbilitySpecFromHandle은 GiveAbility로 허용된 ActivatableAbilities를 순회하며,
	//   GameplayAbilitySpec를 찾아냄
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		// GameplayAbility_FromEquipment는 EquipmentInstance로부터 GiveAbility를 진행했으므로
		// SourceObject에 EquipmentInstance가 저장됨
		return Cast<USampleEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}
