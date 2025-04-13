#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SampleAbilitySystemComponent.generated.h"

/*
	어디에 붙여야 할까?
	Character? -> 그러면 Monster 나 Enemy 같은 녀석들에도 붙여야 하나?
	(일단 전부 붙여야 데미지 계산 등에 사용 가능)
	Controller에 붙여야 하나?

	1. PlayerState에 넣는다
	2. Character에 넣는다
	   - 이 경우는 2가지가 보장되어야 함
	     싱글 게임 + 한번 결정된 Player Character가 바뀌지 않아야 함
		 (일반적인 경우는 PlayerState에 넣는 듯)
		 (Dedicate 기반의 lyra는 player state)

	Dedicate에선 Controller 가 여러모로 핵심
    각 Player Controller가 Player를 대신해 Server에 존재
	이 때, 각 Player Controller를 Player들에게 넘겨주면 안됨
	(controller는 Server에 있어야 하므로)
	(1번 플레이어가 2번 Player Controller를 사용하게 되면 안되므로)

	GameMode는 Server에만 존재
	(이것을 직접적으로 클라에 넘겨주진 않음)

	GameState, PlayerState을 통해 필요한 '상태'를 넘겨주거나
	복사본을 넘겨줌
	GameState : GameMode의 상태를 저장
	PlayerState : PlayerController의 상태를 저장

	각 PlayerController의 정보를 각각의 PlayerState에 담아
	각 클라에 뿌려주기에 각각의 상태를 업데이트 가능
	-> 그러므로 PlayerState에 AbilitySystem을 넣어 서로의 AbilitySystem이 갱신될수있도록 한다

*/
UCLASS()
class SAMPLES_API USampleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	USampleAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

public:
	// Ability Input 처리할 Pending Queue
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles; // 눌렀을 때
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles; // 놓았을 때
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles; // 누르는 동안
};
