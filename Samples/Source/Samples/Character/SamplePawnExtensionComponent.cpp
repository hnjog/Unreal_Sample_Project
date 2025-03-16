#include "SamplePawnExtensionComponent.h"
#include "../SampleLogChannels.h"
#include "Components/GameFrameworkComponentManager.h"

// FeatureName을 Component 단위니, Component는 빼고 PawnExtension으로만 네이밍 한다
const FName USamplePawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

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
	// 
	// FObjectKey? - 특정 클래스가 CDO인지, 일반 객체 인지 등을 파악할 수 있도록
	// 메모리와 ID값이 존재하는 구조체
	// 
	// A 라는 CDO 가 존재하고
	// A1, A2 가 생성되었을때, 각각에 컴포넌트들이 붙을것임
	// 컴포넌트 매니저는 하나밖에 없고 내부에서 Map을 통해 관리한다
	// (같은 클래스들이지만, Instance별로 따로 구분 - FObjectKey 로 구분)
	// (이후 내부에서 FeatureName을 통해 Component를 구분한다)
	//
	// 결론적으로 Manager에게 현재 객체와 자신을 Key로서 등록시키며 관리하도록 요청하는 단계이며,
	// GameInstance 쪽에 정의한 RegisterInitState를 사용하기 위하여 필요한 단계
	RegisterInitStateFeature();
	
	// GetOwningActor
	// - 자신이 Actor라면 자신을,
	//   아니라면 현재 자신이 부착되어있는 액터를 반환
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
