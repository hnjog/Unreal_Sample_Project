#include "SamplePawnExtensionComponent.h"
#include "../SampleLogChannels.h"

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

	// 올바른 Actor에 등록되었는지 확인
	// -> 확인 위치가 OnRegister() 인 이유?
	// OnRegister 단계에서 Component를 부착하기에
	// Component 인, PawnExtension이 생성되어 부착이 되는 상태
	// => GetPawn이 실패한다면 '부착'이 실패된 문제 상황이므로 Error 를 발생시켜준다
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogSample, Error, TEXT("this component has been added to a BP Whose base class is not a Pawn!"));
			return;
		}
	}
}

void USamplePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USamplePawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
