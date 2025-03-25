#include "SampleInputConfig.h"

USampleInputConfig::USampleInputConfig(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UInputAction* USampleInputConfig::FindNativeInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	return nullptr;
}

const UInputAction* USampleInputConfig::FindAbilityInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	return nullptr;
}
