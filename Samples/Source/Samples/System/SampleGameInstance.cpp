#include "SampleGameInstance.h"
#include <Components/GameFrameworkComponentManager.h>
#include <Samples/SampleGameplayTags.h>

void USampleGameInstance::Init()
{
	Super::Init();

	// 게임의 컴포넌트 및 상태 관리 매니저 (modular)
	// 컴포넌트 방식인 경우, '부품으로 부착'하기에
	// FindComponentByClass를 사용하여 얻어올 수 있음
	// 
	// 지금도 유용하지만
	// GameFeature와 함께 사용하면,
	// 특정 인스턴스에 자동으로 컴포넌트를 부착하기에 더욱 유용해진다
	// 
	// 다만 '종속성'이 존재할 수 있기에
	// 그걸 기본적으로는 보장해주지 않음
	// ('순서'대로 붙히진 않으며, 그에 따른 init을 해주진 않음)
	// 
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	// 유효성 확인
	if (ensure(ComponentManager))
	{
		const FSampleGameplayTags& GameplayTags = FSampleGameplayTags::Get();

		// 초기화 상태 순서 등록
		// RegisterInitState(초기화 상태의 태그, 초기화 완료 여부, 완료를 위해 필요한 이전 상태 태그)
		// 초기화 순서를 관리하여, '종속성'에 따라 컴포넌트를 등록시킨다
		// (위에서 말한 '순서'대로 붙이지 않아 Crash가 나는 경우를 예방)
		// RegisterInitState 호출 시점은
		// GameInstance의 '초기화'(Initialization) 단계에서 하라고 엔진에서 명시
		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}

}

void USampleGameInstance::Shutdown()
{
	Super::Shutdown();
}
