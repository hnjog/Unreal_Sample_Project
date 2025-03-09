#include "SampleGameMode.h"
#include"SampleGameState.h"
#include"../Player/SamplePlayerController.h"
#include"../Player/SamplePlayerState.h"
#include"../Character/SampleCharacter.h"

ASampleGameMode::ASampleGameMode()
{
	GameStateClass = ASampleGameState::StaticClass();
	PlayerControllerClass = ASamplePlayerController::StaticClass();
	PlayerStateClass = ASampleGameState::StaticClass();
	DefaultPawnClass = ASampleCharacter::StaticClass();
}

void ASampleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 호출 시점엔 아직 GameInstance를 통해 초기화 작업이 진행중이므로
	// 해당 프레임에선 Lyra의 Comcept인 Experience 처리를 진행할 수 없음
	// - 이를 위해 한 프레임 뒤에 이벤트를 받아 처리를 이어서 진행
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ASampleGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	// 실제 lyra 프로젝트에서는 다양한 환경 설정을 확인하며,
	// dedicate 기반의 프로젝트이기에 이런 함수명을 가지게 됨
	// 현재 프로젝트에선 Experience 로딩을 선택하는 단계로 사용
	// ExperienceManagerComponent을 통해 Experience를 호출시킬 예정
	// -> 위에서 첫 프레임에 바로 호출할땐 해당 Manager가 생성되지 않으므로 한 프레임 늦추는 이유
	//

}
