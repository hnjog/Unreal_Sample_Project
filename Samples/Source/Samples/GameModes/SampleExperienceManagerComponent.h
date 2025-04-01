#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "SampleExperienceManagerComponent.generated.h"

class USampleExperienceDefinition;

enum class ESampleExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExcutingActions,
	Loaded,
	Deactivating,
};

// Unreal Delegate
// Multicast : 하나의 이벤트에 여러 함수 연결 가능
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSampleExperienceLoaded, const USampleExperienceDefinition*);

/*
	USampleExperienceManagerComponent
	- GameState를 owner로 가지며, Experience의 상태 정보를 가지는 Component
	- 추가로 Manager의 역할로서 Experience 로딩 상태 업데이트 및 이벤트를 관리
*/

UCLASS()
class SAMPLES_API USampleExperienceManagerComponent : public UGameStateComponent
{
	// 리플렉션 기능과 관련한 자동 생성 코드

	GENERATED_BODY()
public:
	// 로드 완료 + Experience 존재하는지 체크
	bool IsExperienceLoaded() { return (LoadState == ESampleExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }
	
	// FOnSampleExperienceLoaded::FDelegate - 해당 델리게이트에서 요구하는 함수 타입을 인자로 받는다는 뜻
	// &&(RValue Reference) 
	// - '이동'하여 객체를 복사하는 대신 해당 자원을 그대로 사용함
	// - 리터럴, 람다, 함수반환값 등과 같이 임시 생성된 객체를 포함받을 수 있음
	// &(LValue Reference)
	// - 이 방식은 메모리 상에 존재하는 객체만 받을 수 있음
	// 
	// 아마 RValue인 람다 or std::function 등만 인자로 받을 수 있어 보임
	//
	// OnExperienceLoaded 에 바인딩 하거나, Experience 로딩이 완료되었다면 호출
	void CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();
	const USampleExperienceDefinition* GetCurrentExperienceChecked() const;

public:
	// 가리키는 객체를 수정할 수 없도록 const 를 걸어준다
	// 그래도 ptr은 새로운 Experience를 가르킬 수 있음
	// c++ 의 type* const 방식
	//
	// 로딩 요청을 받아야 로딩을 한다
	UPROPERTY()
	TObjectPtr<const USampleExperienceDefinition> CurrentExperience;

	// Experience의 로딩 상태를 모니터링
	ESampleExperienceLoadState LoadState = ESampleExperienceLoadState::Unloaded;

	// Experience 로딩이 완료된 이후, BroadCasting Delegate
	FOnSampleExperienceLoaded OnExperienceLoaded;

	// 활성화된 GameFeature Plugin들
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
/*
	GameStateComponent (Plugin 기능)
	- UGameStateComponent -> 결국 UActorComponent를 상속받은 녀석
	-> 단순히 Actor에만 붙이는 '컴포넌트'를 GameState 등에 붙일 수 있도록 하는 클래스
	(GameState에 붙이는 추가적인 기능 같은 것)
	(-> Unity에서 다양한 GameObject에 컴포넌트를 붙일 수 있듯)

	GameFeature 시스템기능으로 unity처럼 부품을 붙였다 뗄수 있음
	(확장성이 좋아짐 , 디버깅이 힘들어지고, 부품이 많아질 순 있음)
*/
