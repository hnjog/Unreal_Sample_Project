#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SampleGameState.generated.h"

class USampleExperienceManagerComponent;

UCLASS()
class SAMPLES_API ASampleGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ASampleGameState();

public:
	UPROPERTY()
	TObjectPtr<USampleExperienceManagerComponent> ExperienceManagerComponent;

};

/*
	언리얼의 전반적인 게임 세션
	GameInstance -> World -> Level -> GameMode -> GameState

	GameInstance : 게임이 실행되는 동안 유지되는 데이터를 저장 및 관리 (전반적인 게임의 상태)
	(ex : Player의 데이터)
	World : 게임의 모든 레벨과 내부 오브젝트를 포함 (하나의 게임 세션에서 여러 레벨을 관리)
	(현재 로드된 레벨과 관련된 모든 액터와 컴포넌트를 관리, 레벨 전환 등)
	Level : 실제 게임 환경을 구성 (특정 게임 환경이나 Map)

	GameMode : 레벨에서 게임의 규칙과 흐름을 정의
	GameState : 현재 게임의 상태를 저장 (점수, 승리 조건, 유저의 상태)

*/