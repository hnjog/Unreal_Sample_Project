#include "SampleHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "../SampleLogChannels.h"

const FName USampleHeroComponent::NAME_ActorFeatureName("PawnExtension");

USampleHeroComponent::USampleHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Tick을 꺼준다
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

}

void USampleHeroComponent::OnRegister()
{
	Super::OnRegister();
	// PawnExtensionComponent 와 동일
	// Actor에 제대로 붙었는지 확인하기
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogSample, Error, TEXT("this component has been added to a BP Whose base class is not a Pawn!"));
			return;
		}
	}

	// InitState 사용을 위한 등록 진행
	RegisterInitStateFeature();
}

void USampleHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USampleHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void USampleHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

bool USampleHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager,CurrentState,DesiredState);
}

void USampleHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void USampleHeroComponent::CheckDefaultInitialization()
{
	IGameFrameworkInitStateInterface::CheckDefaultInitialization();
}
