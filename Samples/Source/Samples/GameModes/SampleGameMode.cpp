﻿#include "SampleGameMode.h"
#include"SampleGameState.h"
#include"../Player/SamplePlayerController.h"
#include"../Player/SamplePlayerState.h"
#include"../Character/SampleCharacter.h"
#include"../GameModes/SampleExperienceManagerComponent.h"
#include"Samples/SampleLogChannels.h"

ASampleGameMode::ASampleGameMode()
{
	// GameState를 설정해주었기에 World가 생성되는 시점에 생성자로 ExperienceManager를 생성
	GameStateClass = ASampleGameState::StaticClass();
	PlayerControllerClass = ASamplePlayerController::StaticClass();
	PlayerStateClass = ASamplePlayerState::StaticClass();
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

void ASampleGameMode::InitGameState()
{
	Super::InitGameState();
	
	// GameState 내부에 해당 클래스 존재시 반환
	// Experience 비동기 로딩을 위한 Delegate를 준비
	USampleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<USampleExperienceManagerComponent>();
	// InitGameState 가 호출된 시점에는 ExperienceManagerComponent 가 생성되어있음
	// (GameState 에서 생성하면서 해당 클래스를 생성해놓았으므로)
	check(ExperienceManagerComponent);

	// OnExperienceLoaded를 등록
	// 로딩이 되어있다면 바로 호출하고, 아니면 기다렸다 호출된다
	// 현재 시점에선 로드가 안되어있으므로 기다렸다 호출하기 위함
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate::CreateUObject(this,&ThisClass::OnExperienceLoaded ));
}

void ASampleGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (isExperienceLoaded())
	{
		// experience가 load 된 이후에 pawn을 생성할 목적
		// 그리고 load 완료후 다시 이 함수를 호출
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ASampleGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UE_LOG(LogSample, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer,SpawnTransform);
}

void ASampleGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	// 실제 lyra 프로젝트에서는 다양한 환경 설정을 확인하며,
	// dedicate 기반의 프로젝트이기에 이런 함수명을 가지게 됨
	// 현재 프로젝트에선 Experience 로딩을 선택하는 단계로 사용
	// ExperienceManagerComponent을 통해 Experience를 호출시킬 예정
	// -> 위에서 첫 프레임에 바로 호출할땐 해당 Manager가 생성되지 않으므로 한 프레임 늦추는 이유
	//

	// 해당 함수에서 우리가 로딩할 Experience에 대해 PrimaryAssetId를 생성하여
	// OnMatchAssignmentGiven으로 넘겨준다

	// PrimaryAsset : '중요한 자산'이란 뜻으로
	// Unreal의 자산 시스템의 일부
	// PrimaryAsset은 로드 및 관리하는 시스템이 통합되어 있기에
	// 핵심 에셋들을 효율적으로 로드 & 언로드 할 수 있음
	// (일반 Asset 은 UObject)
	// 
	// FPrimaryAssetId : PrimaryAsset을 구별하기 위한 식별자이며
	// 내부에 AssetType과 AssetName을 가진다
	// (일종의 FName 변수이며, 각각 구별을 위한 태그로 사용자가 임의로 지정 가능)
	//
	FPrimaryAssetId ExperienceId;

	// precedence order(highest wins) -> 우선순위
	// - matchmaking assignment(if present)
	// - default experience

	UWorld* World = GetWorld();

	// fall back to the default experience
	// 기본 옵션으로 default하게 B_SampleDefaultExperience로 설정해놓기
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("SampleExperienceDefinition"), FName("B_SampleDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

// 선택한 ExperienceId를 서버로 넘겨준다
void ASampleGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// ExperienceManagerComponent를 활용하여 Experience를 로딩하기 위해
	// ExperienceManagerComponent의 ServerSetCurrentExperience 를 호출
	check(ExperienceId.IsValid());

	check(GameState);
	USampleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<USampleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// 네트워크 상에선 게임 모드가 Client가 아니라 Server에 존재
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

bool ASampleGameMode::isExperienceLoaded() const
{
	check(GameState);
	USampleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<USampleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ASampleGameMode::OnExperienceLoaded(const USampleExperienceDefinition* currentExperience)
{

}
