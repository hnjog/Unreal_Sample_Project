#include "SampleHealthComponent.h"
#include "Samples/AbilitySystem/SampleAbilitySystemComponent.h"
#include "Samples/AbilitySystem/Attributes/SampleHealthSet.h"
#include "Samples/SampleLogChannels.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

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

void USampleHealthComponent::InitializeWithAbilitySystem(USampleAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogSample, Error, TEXT("SampleHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	// ASC 캐싱
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogSample, Error, TEXT("SampleHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	// AbilitySystemComponent::GetSet은 SpawnedAttributes에서 가져옴
	// - 근데 PlayerState에서 Subobject로 생성하고 따로 ASC에 등록한적이 없음
	//   - AbilitySystemComponent::InitializeComponent()에서 GetObjectsWithOuter로 SpawnedAttributes에 추가
	//   - HealthSet은 PlayerState에 Subobject로 있고, ASC 또한 PlayerState에 존재
	//     -> 이는 ASC에서 GetObjectsWithOuter로 HealthSet이 접근
	// - AbilitySystemComponent::InitializeComponent() 체크
	HealthSet = AbilitySystemComponent->GetSet<USampleHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogSample, Error, TEXT("SampleHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// GetGameplayAttributeValueChangeDelegate 를 통하여
	// 특정 Attribute의 변화에 따라 Callback 함수 등록
	// 
	// HealthSet의 HealthAttribute의 업데이트가 일어날때마다
	// 호출할 콜백으로 멤버메서드 HandleHealthChanged를 등록
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USampleHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);

	// 초기화 한번 해줬으니 Broadcast
	OnHealthChanged.Broadcast(this, 0, HealthSet->GetHealth(), nullptr);
}

void USampleHealthComponent::UninitializeWithAbilitySystem()
{
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	// GameEffectModifier에 Data가 있는 경우에만 호출
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}
	return nullptr;
}

// 체력 바뀌었으므로 BroadCast
void USampleHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}
