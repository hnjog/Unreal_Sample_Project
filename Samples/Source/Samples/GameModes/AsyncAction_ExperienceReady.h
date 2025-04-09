#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

// BP를 통한 UFunction으로 바인딩하는 Multicast Delegate: Dynamic!
// (BP에서 사용하려면 Dynamic을 사용해야 하기에)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class AGameStateBase;
class USampleExperienceDefinition;

// Experience 가 로딩이 완료되었는지를 체크하는것이 목적인 클래스
// (BP 사용할때, Experience의 로딩이 되었는지 안되었는지)
UCLASS()
class SAMPLES_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출
	// WaitForExperienceReady는 static이고 Blueprint에서 부를 수 있음
	// 이걸 호출하고 넘겨받은 포인터로 작업
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);
	
	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	
	// Step1 - Step4
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const USampleExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();

	// BlueprintAssignable은 BP상에서 할당 가능한 변수로 정의
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	// WorldPtr을 단순 로직상 캐싱하는 용도
	TWeakObjectPtr<UWorld> WorldPtr;
};
