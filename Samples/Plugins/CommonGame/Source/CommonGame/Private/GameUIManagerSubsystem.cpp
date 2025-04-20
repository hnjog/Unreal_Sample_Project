#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// CurrentPolicy가 설정되어 있지 않고, DefaultUIPolicyClass가 제대로 설정된 경우
	if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull())
	{
		// DefaultUIPolicyClass 는 BP 에셋이기에 Load 필요
		TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();

		// NewObject로 인스턴싱 하여 설정
		SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
	}
}

void UGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// Dedicate Server의 경우, GameUIManagerSubsystem을 활성화하지 않음
	// Server에서 UI를 쓸일이 없으니
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// 어떠한 Subsystem도 UGameUIManagerSubsystem을 상속받지 않았다면,
		// 해당 GameInstanceSubsystem은 활성화 x
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		return ChildClasses.Num() == 0;
	}
	return false;
}
