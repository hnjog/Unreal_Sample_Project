#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "SampleInputConfig.h"
#include "EnhancedInputComponent.h"
#include "SampleInputComponent.generated.h"


UCLASS()
class SAMPLES_API USampleInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	USampleInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	template<class UserClass, typename FuncType>
	void BindNativeAction(const USampleInputConfig* InputConfig, const FGameplayTag& InputTag,ETriggerEvent TriggerEvent,UserClass* Object, FuncType Func, bool bLogIfNotFound);
	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const USampleInputConfig* InputConfig, const FGameplayTag& InputTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
inline void USampleInputComponent::BindNativeAction(const USampleInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
inline void USampleInputComponent::BindAbilityActions(const USampleInputConfig* InputConfig, const FGameplayTag& InputTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
}
