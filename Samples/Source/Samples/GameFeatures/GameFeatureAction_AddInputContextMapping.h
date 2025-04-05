#pragma once

#include "GameFeatureAction_WorldActionBase.h"
#include "UObject/SoftObjectPtr.h"
#include "GameFeatureAction_AddInputContextMapping.generated.h"

class UInputMappingContext;
struct FComponentRequestHandle;

USTRUCT()
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AssetBundles = "Client,Server"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 Priority = 0;

	/** If true, then this mapping context will be registered with the settings when this game feature action is registered. */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bRegisterWithSettings = true;
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Input Mapping"))
class UGameFeatureAction_AddInputContextMapping final : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

	// UGameFeatureAction Interface
public:
	// 기본적으로 특정한 게임 기능이 활성화 되거나 비활성화 될때 호출되는 함수들
	// ex : 레벨 로드, 게임 모드 변경, 게임 세션 시작, 플러그인 활성화 
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	// ex : 레벨 언로드, 게임 모드 변경, 게임 세션 종료, 플러그인 비활성화 
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	// UGameFeatureAction_WorldActionBase Interface
public:
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	// Member Variable
public:
	// 각각의 컨텍스트에 대한 관련 데이터
	// GameFeature가 엔진 쪽의 개념이기에
	// 적용할 World에 대한 데이터를 가져다 캐싱해 놓는 느낌
	// 일일이 전부 World 순회하지 않기 위해
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
	};

	// UInputMappingContext 을 WeakPtr로 들고 있는 구조체
	// 그것을 array로 가지고 있는다
	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<FInputMappingContextAndPriority> InputMappings;

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	FDelegateHandle RegisterInputContextMappingsForGameInstanceHandle;

	// Member Method
public:
	// ActiveData 를 초기화
	// Engine Subsystem은 PI가 사라져도 리셋되지 않기에
	// 매핑을 끊기 위한 함수
	void Reset(FPerContextData& ActiveData);

	// GameFeatureState가 변경될 때의 콜백 함수
	// (or ExtensionEvent를 GameFrameworkComponentManager를 통해 호출 가능)
	void HandleControllerExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	// UEnhancedInputLocalPlayerSubsystem 에
	// MappingContext를 추가
	void AddInputMappingForPlayer(UPlayer* Player, FPerContextData& ActiveData);
	void RemoveInputMapping(APlayerController* PlayerController, FPerContextData& ActiveData);
};

/*
	Event? Delegate?
	Delegate 는 '구독'하는 방식
	-> 특정한 존재에게 그러한 '상황'을 원하는 개체들이
	   찾아가는 형식
	   (전체적인 실행 보장)

	Event는 위와 비슷하지만,
	EventName을 통하여 필요한 개체들만 실행 시킬 수 있음
*/