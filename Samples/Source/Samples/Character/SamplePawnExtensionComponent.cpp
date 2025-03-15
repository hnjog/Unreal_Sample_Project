#include "SamplePawnExtensionComponent.h"

USamplePawnExtensionComponent::USamplePawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Tick을 꺼준다
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USamplePawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
}

void USamplePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USamplePawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
