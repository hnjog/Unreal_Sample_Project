#include "SampleHUD.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SampleHUD)

ASampleHUD::ASampleHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void ASampleHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// SampleHUD를 Receiver로 Actor 추가
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASampleHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ASampleHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
