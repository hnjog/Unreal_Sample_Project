#pragma once

#include "Containers/Map.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_WorldActionBase.generated.h"

class FDelegateHandle;
class UGameInstance;
struct FGameFeatureActivatingContext;
struct FGameFeatureDeactivatingContext;
struct FWorldContext;

// 어떠한 PI (World)가 호출하는지 모르기에
// context로 구분하는 용도의 래퍼 클래스
UCLASS(Abstract)
class UGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	// GameFeatureAction's interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

	// interface
	// 
	// PURE_VIRTUAL : C++의 Interface 기능이 강하게 없기에
	// 구현된 Unreal용 매크로 (상속 받는 클래스는 이 함수를 반드시 구현해야 함)
	//  - 비슷한 C++ 의 순수 가상 함수와 비교하자면?
	//  ex ) virtual void a() = 0;
	//  공통점 : 직접 인스턴스 생성 x, 반드시 상속받는 클래스에서 구현해야 함
	//  차이점 : PURE_VIRTUAL는 내부적으로 빈 구현을 생성하고, 디버깅 정보를 제공
	//  (C++의 경우는 호출될 시, 런타임 오류 발생)
	//  (PURE_VIRTUAL는 호출될 시 메시지나 로그를 출력)
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );
};