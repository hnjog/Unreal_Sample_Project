#include "SamplePawnExtensionComponent.h"
#include "../SampleLogChannels.h"
#include "Components/GameFrameworkComponentManager.h"
#include"../SampleGameplayTags.h"

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
	// UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void USamplePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// FeatureName에 NAME_None을 넣으면, Actor에 등록된 Feature Component의 InitState 상태를 관찰하겠다는 의미
	// Actor의 InitState가 변경되는 상황을 구독받겠다
	// NAME_None와 FGameplayTag (빈 태그) 인 상황
	// 
	// NAME_None 을 넣으면 현재 자신 이후로 생길 다른 컴포넌트 들에 대하여 상태 변화를 감지한다는 의미
	// (정확히는 PawnExtensionComponent가 먼저 생성이 되고,
	//  다른 컴포넌트들은 같은 객체의 다른 컴포넌트로 Manager가 던져줄테니)
	// (결국 ComponentManager가 알려준다)
	// (특정한 FeatureName을 정확히 넣으면, 해당하는 FeatureName의 상태 변경만 받을 수 있음)
	//
	// 현재는 '빈 값'을 넣었기에 모든 상태 변화를 감지한다
	// FGameplayTag 에 특정한 상태를 넣는 경우,
	// 해당 상태로 변경되는 것만 받게 된다
	//
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// InitState_Spawned 로 상태 변환 시도
	// - TryToChangeInitState 의 진행 양상
	//  1. CanChangeInitState로 상태 변환의 가능성 유무를 판단
	//  2. HandleChangeInitState로 내부 상태를 변경(Feature Component)
	//  3. BindOnActorInitStateChanged로 Bind된 Delegate를 조건에 맞게 호출
	//		(현재 객체가 변경된 상태에 구독한 Delegate 들에게 알려준다)
	//		- SamplePawnComponent의 경우, 모든 Actor의 Feature 상태 변화에 따라
	//		  OnActorInitStateChanged 가 호출됨 (위에서 Bind 했으므로)
	ensure(TryToChangeInitState(FSampleGameplayTags::Get().InitState_Spawned));

	// 오버라이드 하여 사용하는 함수
	// - ForceUpdateInitState 와 같이 강제 업데이트 진행
	//   (CanChangeInitState와 HandleChangeInitState 를 통하여)
	CheckDefaultInitialization();
}

void USamplePawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 소멸 시점에 호출, RegisterInitStateFeature() 와 쌍을 이루는 함수
	// 이걸 빠트리면, 컴포넌트가 제거되지 않고 map에 계속 남아 유지될 가능성도 존재함
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void USamplePawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

bool USamplePawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void USamplePawnExtensionComponent::CheckDefaultInitialization()
{
	IGameFrameworkInitStateInterface::CheckDefaultInitialization();
}
