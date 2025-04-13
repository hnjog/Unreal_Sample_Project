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
	void BindAbilityActions(const USampleInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
inline void USampleInputComponent::BindNativeAction(const USampleInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	// InputConfig는 활성화 가능한 InputAction을 담고 있음
	// - InputConfig에 없는 InputAction을 Binding 시키면, nullptr 반환하기에 Bind 실패
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		// BindAction : InputAction을 Delegate에 연결
		// 내부에서 FEnhancedInputActionEventDelegateBinding 객체를 생성하고,
		// 지정된 InputAction과 TriggerEvent를 바인딩 (TUniquePtr로 관리)
		// 
		// InputAction에 BindDelegate를 설정하여 Object와 FunctionName에 해당하는 함수를 바인딩
		//
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
inline void USampleInputComponent::BindAbilityActions(const USampleInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	// 모든 InputAction에 다 바인딩
	for (const FSampleInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc,Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
