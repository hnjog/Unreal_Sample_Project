#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SampleGameMode.generated.h"

UCLASS()
class SAMPLES_API ASampleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASampleGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	void HandleMatchAssignmentIfNotExpectingOne();

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