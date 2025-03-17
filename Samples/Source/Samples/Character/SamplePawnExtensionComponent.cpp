#include "SamplePawnExtensionComponent.h"
#include "../SampleLogChannels.h"
#include "Components/GameFrameworkComponentManager.h"
#include"../SampleGameplayTags.h"
#include"../Character/SamplePawnData.h"

// FeatureName을 Component 단위니, Component는 빼고 PawnExtension으로만 네이밍 한다
const FName USamplePawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

USamplePawnExtensionComponent::USamplePawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Tick을 꺼준다
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USamplePawnExtensionComponent::SetPawnData(const USamplePawnData* InPawnData)
{
	// Pawn에 대한 Authority가 없으면 이 함수는 진행하지 않음
	// GetLocalRole -> 객체의 네트워크 역할을 확인하는 데 사용하는 객체
	// 서버 / 클라 로 나뉘는 네트워크의 역할에서 역할을 구분한다
	// ROLE_Authority : 서버가 해당 객체를 제어함 (서버가 게임 객체를 관리 및 변경)
	// 
	// ROLE_AutonomousProxy : 클라이언트가 객체를 제어할 수 있음 
	// (클라가 자신의 캐릭터 등을 조작하는 상황 -> 차후, 서버에서 확인하고 수정 이나 동기화)
	// 
	// ROLE_SimulatedProxy : 서버에서 제어되나, 클라이언트가 이를 시뮬레이션 하는 경우
	// (NPC, 다른 플레이어, 몬스터와 같이 서버에서 상태를 제어하고,
	// 클라에선 해당 위치를 통해 렌더링 및 구현 되는 경우)
	// 
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
		return;

	if (PawnData)
		return;

	PawnData = InPawnData;
	// lyra 원본은 내부에선
	// CheckDefaultInitialization를 호출하나, 
	// 해당 시점에선 State가 None이기에 StateChain이 호출되어도 상태가 변하지 않는다 
	// -> 캐릭터가 생성되고 바로 컴포넌트에 넣어주었으며,
	// BeginPlay()는 객체가 월드에 스폰되고 '다음 프레임'에 호출이 된다
	// (여기서 넣어봤자 BeginPlay 호출이 안된다 -> 상태 정의가 None 상태)
	// (없는 상태를 따로 StateChain에 넣어놓지 않았기에 진행되지 않음)
	// 
}

void USamplePawnExtensionComponent::SetupPlayerInputComponent()
{
	// Update를 통한 InitState의 상태 변환을 시작
	CheckDefaultInitialization();
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
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// PawnExtensionComponent에선 다른 Feature Component들의 상태가
		// InitState_DataAvailable 임을 관찰하고 싱크를 맞춰줌 (CanChangeInitState)
		// - 이를 위해 OnActorInitStateChanged 에서 
		// InitState_DataAvailable 로 바뀐 컴포넌트 들이 있으면
		// 지속적으로 CheckDefaultInitialization 를 호출시켜
		// 전체적인 상태를 확인하고 갱신한다
		// 
		// PEC::CheckDefaultInitialization -> 
		// 다른 컴포넌트들의 CheckDefaultInitialization 호출 및
		// ContinueInitStateChain 를 통해
		// CanChangeInitState 를 내부에서 호출시킨다
		// CanChangeInitState 와 HandleChangeInitState 를 통해
		// 상태가 변경되면 OnActorInitStateChanged 가 호출된다
		const FSampleGameplayTags& InitTags = FSampleGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

// CurrentState -> DesiredState로 바꾸기 위하여 '요청'하는 함수
bool USamplePawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	// 일단 OnRegister에서 체크하고 있으니 Pawn 체크는 안한듯?
	APawn* Pawn = GetPawn<APawn>();
	
	const FSampleGameplayTags& InitTags = FSampleGameplayTags::Get();

	// InitState_Spawned 초기화 상태 요청
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn이 잘 세팅만 되어있으면 바로 Spawned로 넘어감!
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// 아마 PawnData를 누군가 설정하는 모양이다
		// -> GameMode에서 해준다
		if (!PawnData)
		{
			return false;
		}

		// LocallyControlled인 Pawn이 Controller가 없으면 에러!
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor에 바인드된 모든 Feature들이 DataAvailable 상태일 때, DataInitialized로 넘어감:
		// - HaveAllFeaturesReachedInitState 확인
		// 다른 컴포넌트들과 비슷하게 연동되도록
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	// 위의 선형적인(linear) transition이 아니면 false!
	return false;
}

// PawnExtension에서
// 나를 제외하고 전부 CheckDefaultInitialization 를 호출 시킨 후,
// ContinueInitStateChain 를 통해 현재 가능한 상태까지 업데이트를 하게 된다
void USamplePawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComponent는 Feature Component들의 초기화를 관장하는 Component이다:
	// - 따라서, Actor에 바인딩된 Feature Component들에 대해 CheckDefaultInitialization을 
	//   호출해주도록 한다 (ForceUpdate 느낌?)
	//   (현재 자신은 제외하고)
	// - 이 메서드를 IGameFrameworkInitStateInterface가 제공하는데, 
	//    CheckDefaultInitializationForImplementers
	// 
	CheckDefaultInitializationForImplementers();

	const FSampleGameplayTags& InitTags = FSampleGameplayTags::Get();

	// 사용자 정의 InitState를 직접 넘겨준다
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// CanChangeInitState()와 HandleChangeInitState() 
	// 그리고 ChangeFeatureInitState 호출을 통한 OnActorInitStateChanged Delegate 호출까지 진행
	// 
	// chain들을 받고
	// 현재 상태에서 다음 상태로 넘어갈 수 있는지를 판단하고
	// 가능하면 계속 넘겨준다
	ContinueInitStateChain(StateChain);
}
