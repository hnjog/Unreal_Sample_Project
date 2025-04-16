#pragma once

#include "Containers/Array.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"
#include "SampleGameFeaturePolicy.generated.h"

// GameFeature Plugin이 메모리에 로딩되거나 Active(활성화)를 관리하는
// StateMachine을 모니터링 가능하다
// UDefaultGameFeaturesProjectPolicies
// - 각 상황에 맞게 GameFeature Subsystem을 동작하도록 정책을 만든 클래스
//
UCLASS()
class USampleGameFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
public:
	USampleGameFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// GameFeaturesProjectPolicies Interface
	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;

public:
	// Observer로서 등록한 객체 관리
	// Observer 패턴
	// (변화를 감시할 녀석을 등록하여, 대상이 변경시 알림을 받음)
	// (Delegate와 유사한 디자인 패턴)
	// 
	// Transient : 게임이 저장되거나 로드될 때 직렬화 하지 않음
	// (게임 세션이 종료되거나 불러올 때, 이 속성의 값은 유지되지 않음)
	// => 일시적인 데이터 를 저장할 때 유용 (메모리 최적화)
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};

/*
	GameFeature Plugin이 Register/Unregister 단계에서
	GameplayCuePath가 등록되어, GameplayCueManager가 관련 GameplayCue 에셋을 스캔할 수 있도록 함

	이를 위해 IGameFeatureStateChangeObserver 를 활용해,
	옵저버 패턴으로 이를 가능하도록 함
	(GameFeatureSubSystem에 등록 가능)

	(GameFeatureAction 에도 Register/Unregister가 존재)
	(-> 그러나 직접 호출해줘야 함)
	(Lyra에선 GameFeaturePolicy를 오버라이딩 하는 방식으로 이를 진행)

	GameFeature에서 Scan에 의하여
	Register, UnRegister 되었을 때
	해당 함수들에 알림을 받음

	CueManager는 AssetManager와 용도가 다름
	하나의 Gameplay Cue 가 있을 때
	CueManager에 등록함으로, Tag를 통해 찾아올 수 있음

	Cue 자체도 UObject 이기에
	메모리에 올라오며, AssetManager에서 관리한다
	(생각보다 로딩이 오래걸리기에 Engine Subsystem에서 처리하는 것이
	그나마 로딩을 덜 체감되게 한다)

	Cue Manager를 상속받아 AssetManager에 등록시키는 Class를 만들 예정

*/
UCLASS()
class USampleGameFeatureAction_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()
	// IGameFeatureStateChangeObserver Interface
public:
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL);
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL);
};