#include "SampleGameMode.h"
#include"SampleGameState.h"
#include"../Player/SamplePlayerController.h"
#include"../Player/SamplePlayerState.h"
#include"../Character/SampleCharacter.h"
#include"../Character/SamplePawnData.h"
#include"../GameModes/SampleExperienceManagerComponent.h"
#include"../GameModes/SampleExperienceDefinition.h"
#include"Samples/SampleLogChannels.h"
#include"../Character/SamplePawnExtensionComponent.h"
#include <Kismet/GameplayStatics.h>

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
	// InitGameState가 호출된 후, HandleMatchAssignmentIfNotExpectingOne 가 호출된다
	// InitGame(이 떄 한 프레임 뒤에 HandleMatchAssignmentIfNotExpectingOne 호출) 
	// -> GameState 생성자 호출(ExperienceManager 생성) -> InitGameState 호출로 인하여
	// OnExperienceLoaded 등록(이 때, Pawn들을 Restart 시킴)
	// 
	// HandleMatchAssignmentIfNotExpectingOne 를 통해 한 프레임 뒤,
	// Manager에게 Experience를 Load하도록 시키며,
	// 완료 될시 OnExperienceLoaded 가 호출
	//
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
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ASampleGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetDefaultPawnClassForController 를 활용해
	// PawnData로부터 PawnClass를 가져옴
	if (const USamplePawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	// 원래는 여기서 생성자에서 등록된 ASampleCharacter를 통해 SampleCharacter를 세팅하지만
	// Experience가 load 되었다면 그걸 가져온다
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

PRAGMA_DISABLE_OPTIMIZATION
void ASampleGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (isExperienceLoaded())
	{
		// experience가 load 된 이후에 pawn을 생성할 목적
		// 그리고 load 완료후 다시 이 함수를 호출
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}
PRAGMA_ENABLE_OPTIMIZATION

APawn* ASampleGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	// 원래 있었던 것들
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	// PawnData를 가져가 캐싱하기 위한 일종의 Wrapping
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass,SpawnTransform,SpawnInfo))
		{
			if (USamplePawnExtensionComponent* PawnExtComp = USamplePawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const USamplePawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
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

	// HostSession에서 넘겨준
	// URL과 함께 ExtraArgs로 넘겼던 정보가 OptionsString에 저장되어 있음
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		// Experience의 Value를 가져와서 PrimaryAssetId를 생성해줌
		// 이 떄 SampleExperienceDefinition의 class 이름을 사용
		// 
		// OptionsString?
		// - 서버와 클라이언트 간의 데이터 전송에 사용되는 문자열 (추가적인 설정이나 파라미터를 전달하는 용도)
		// 
		// 기본적으로 HostSession에서 ServerTravel을 호출하고,
		// 기존 레벨이 언로드된 후, 새로운 레벨을 Load함
		// 이후 게임모드를 초기화하며 InitGame을 호출하게 됨
		// URL은 모든 클라이언트에 전달되기에 모든 클라이언트가 동일한 설정을 할 수 있음
		// - 여담으로 로컬 환경에서는 에디터가 임의의 서버 인스턴스를 생성하고 이를 통해 ServerTravel이 사용됨
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(USampleExperienceDefinition::StaticClass()->GetFName(), FName(*ExperienceFromOptions));
	}

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
	// PlayerController 순회
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		// Load가 지금 완료되었지만,
		// 먼저 Spawn되어 아무것도 하지 못한 Case가 있을 수 있음
		// 그렇기에 전부 순회하면서 한번씩 체크

		// PC가 Pawn을 Possess하지 않았다면, RestartPlayer을 통해 Pawn을 다시 Spawn
		// - Onpossess를 참고
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				// 재시작?
				// 스타트 지점을 찾고,
				// 내부적으로 Pawn을 가져오고 Spawn 해준다
				// 이후 SetPawn, 그리고 Possess를 진행
				// 사실상 초기에 Player 생성되는 로직을 다시 진행하는 것과 같다
				// 
				// 우리는 Experience가 로드될때까지 Pawn 생성을 막아뒀기에
				// 초반에 해당 로직이 실행되지 못하는데
				// 그것을 Restart를 통해 다시 진행시켜줌
				RestartPlayer(PC);
			}
		}
	}
}

const USamplePawnData* ASampleGameMode::GetPawnDataForController(const AController* InController) const
{
	// 게임 도중에 PawnData가 오버라이드 되었을 경우,
	// PawnData는 PlayerState에서 가져오게 됨
	if (InController)
	{
		if (const ASamplePlayerState* SamplePS = InController->GetPlayerState<ASamplePlayerState>())
		{
			// 현재 캐싱이 되어있는 PawnData가 이미 있다면
			// 그대로 반환한다 (이 경우는 playerState에서 바로 사용하는 것과 똑같긴 하다)
			if (const USamplePawnData* PawnData = SamplePS->GetPawnData<USamplePawnData>())
			{
				return PawnData;
			}
		}
	}

	// fall back to the default for the current experience
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우,
	// ExperienceManagerComponent의 CurrentExperience로부터 가져와서 설정한다
	check(GameState);
	USampleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<USampleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// load가 완료 되었다면 현재 Experience 쪽으로 가서 가져옴
	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const USampleExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// load가 된것도 아닌데 여기 들어왔네?
	return nullptr;
}
