#include "SamplePlayerState.h"
#include"../GameModes/SampleExperienceDefinition.h"
#include"../Character/SamplePawnData.h"
#include"../GameModes/SampleExperienceManagerComponent.h"
#include"../GameModes/SampleGameMode.h"

void ASamplePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	USampleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<USampleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ASamplePlayerState::OnExperienceLoaded(const USampleExperienceDefinition* CurrentExperience)
{
	if (ASampleGameMode* GameMode = GetWorld()->GetAuthGameMode<ASampleGameMode>())
	{
		// ASampleGameMode에서 GetPawnDataForController를 구현해야 한다
		// - GetPawnDataForController 에서 아직 PawnData를 설정하지 않았기에
		// ExperienceManager의 DefaultPawnData로 설정
		//
		// CurrentExperience 내부에 DefaultPawnData가 존재하는데
		// 굳이 GameMode를 경유해서?
		// 
		// 1. 안정성 검사
		// - Experience 로딩이 완료된 시점이긴 하나
		//   혹시 모르니 GameState를 통해 Manager에 CurrentExperience가 제대로 로드 되었는지를 확인하기 위함
		//   (처음 들어온 시점에선 자신의 PawnData는 비어있을 테니,
		//	  GameState의 GetPawnDataForController 내부에서
		//	  ExperienceManager 의 Loaded 함수를 통해 로딩 여부를 다시 체크한다)
		// 
		// 2. PawnData를 가져오는 함수를 통일시킴
		//  - 현 시점에선 PlayerState가 사용하지만
		//    GameMode, Experience, PlayerState가 모두 PawnData를 각각 사용한다면 
		//    각자의 호출이 다르기에 유지보수 비용이 더 들어가게 됨
		//

		const USamplePawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ASamplePlayerState::SetPawnData(const USamplePawnData* InPawnData)
{
	check(InPawnData);
	check(!PawnData); // 2번 설정되지 않게 하기 위해
	PawnData = InPawnData;
}
