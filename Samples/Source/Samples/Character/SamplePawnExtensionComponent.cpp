#include "SamplePawnExtensionComponent.h"
#include "../SampleLogChannels.h"
#include "Components/GameFrameworkComponentManager.h"

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

	// GameFrameworkComponentManager의 InitState 사용을 위한 등록 진행
	// - IGameFrameworkInitStateInterface를 상속받았기에 RegisterInitStateFeature 을 통해 등록
	// InitState를 사용하려면 '등록'을 해야 한다
	// 
	// UGameFrameworkComponentManager의 GetForActor 를 보면
	// 특정한 World마다 하나만 존재 (싱글톤과 비슷)
	// 
	// RegisterInitStateFeature
	// - IGameFrameworkInitStateInterface 인터페이스 구현시,
	//   자신의 상태를 매니저에 등록시키는 함수
	//   (컴포넌트 자신과 '소유 액터'를 매니저에 등록함으로서,
	//    Actor가 GameFeature로 정의된 현재 기능의 이름으로 현재 객체(컴포넌트)를 등록)
	RegisterInitStateFeature();
	
	// 디버깅을 위한 함수
	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void USamplePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USamplePawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
