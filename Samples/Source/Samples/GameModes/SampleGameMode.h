#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SampleGameMode.generated.h"

class USampleExperienceDefinition;

UCLASS()
class SAMPLES_API ASampleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASampleGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() final;

	// 블루프린트 네이티브 이벤트
	// - C++에서 정의된 함수를 블루프린트에서 사용할 수 있도록 하는 유형의 함수
	// - _Implementation 앞쪽 부분의 함수로 선언되며, (ex : HandleStartingNewPlayer)
	//  해당 함수 호출 시 _Implementation 로 구현된 부분이 실행된다
	// -> C++과 블루프린트간의 유연한 통합으로 '성능'과 쉬운 호출을 위한 기능
	// 
	// (_Implementation 가 구현이 되어 있지 않은 경우는 BP의 원본 방식을 사용하고,
	//  구현된 경우는 C++을 우선적으로 사용함)
	// 
	// HandleStartingNewPlayer
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	// SpawnDefaultPawnAtTransform
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

	// member method
	void HandleMatchAssignmentIfNotExpectingOne();
	bool isExperienceLoaded() const;
	void OnExperienceLoaded(const USampleExperienceDefinition* currentExperience);
};

/*
	GameMode
	- GameState
	 : 일반적으로는 GameMode와 1:1 대응
	 Lyra 내부에선 게임 모드 안에 여러 게임이 존재하고 
	 이를 Experience로 대응
	 이러한 Experience 전환을 담당 ManagerComponent 클래스로 대응

	- PlayerState
	  : 폰 데이터 캐싱

	- PlayerController
	  : CameraManager에 대한 소유권을 가지도록

	- DefaultPawn
	 - Character
		- PawnExtension : Character에 대한 정보를 가지는 클래스
		- CameraComponent : Character의 Camera 속성 및 기능을 가지는 클래스
							(PlayerController에 의해 업데이트를 진행시킴)
		각 캐릭터 특성을 살리려면 이 클래스에 부품을 붙이는 쪽으로
*/