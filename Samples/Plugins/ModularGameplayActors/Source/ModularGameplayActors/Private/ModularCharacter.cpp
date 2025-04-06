#include "ModularCharacter.h"
#include"Components/GameFrameworkComponentManager.h"

void AModularCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// GameFeature에서 AddComponent 등이 가능하도록 등록
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularCharacter::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 순서가 반대인 경우는 간헐적으로 crash가 날 수 있으므로,
	// EndPlay는 가장 마지막에 호출하자
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
