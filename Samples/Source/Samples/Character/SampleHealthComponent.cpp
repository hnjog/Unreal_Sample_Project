#include "SampleHealthComponent.h"

USampleHealthComponent::USampleHealthComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	AbilitySystemComponent(nullptr), // InitializeWithAbilitySystem으로 ASC 초기화 이전에는 nullptr 로 세팅
	HealthSet(nullptr)
{
	// 중간다리 역할로 이벤트 기반 작동할 클래스이기에
	// Tick으로 업데이트할 필요는 없음
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}
