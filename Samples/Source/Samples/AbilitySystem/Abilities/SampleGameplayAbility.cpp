#include "SampleGameplayAbility.h"

USampleGameplayAbility::USampleGameplayAbility(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ActivationPolicy = ESampleAbilityActivationPolicy::OnInputTriggered;
}
