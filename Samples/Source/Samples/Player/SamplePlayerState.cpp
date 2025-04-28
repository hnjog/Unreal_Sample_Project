#include "SamplePlayerState.h"
#include "Samples/GameModes/SampleExperienceDefinition.h"
#include "Samples/Character/SamplePawnData.h"
#include "Samples/GameModes/SampleExperienceManagerComponent.h"
#include "Samples/GameModes/SampleGameMode.h"
#include "Samples/AbilitySystem/SampleAbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Samples/AbilitySystem/SampleAbilitySet.h"
#include "Samples/AbilitySystem/Attributes/SampleHealthSet.h"
#include "Samples/AbilitySystem/Attributes/SampleCombatSet.h"

ASamplePlayerState::ASamplePlayerState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// CDO를 이용한 생성
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<USampleAbilitySystemComponent>(this, TEXT("AbilitySystem"));

	// 캐싱을 별도로 하지 않는다?
	// 
	// AbilitySystemComponent 이 Init 되는 과정에서
	// AbilitySystemComponent 의 Outer (지금은 this를 넣어줌, 즉 PlayerState)
	// 의 Attribute를 알아서 가져온다
	// (그렇기에 생성만 해두고, 별도로 캐싱하지 않는 것)
	// (매개변수로 지정하지 않으면 outer로 this인 playerState가 지정)
	// 
	// 이러한 Data를 PlayerState에서 관리
	// (Client, Server 양쪽 모두에서 존재하며
	//  Pawn처럼 죽는 경우에도 사라지지 않게 하기 위하여)
	CreateDefaultSubobject<USampleHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<USampleCombatSet>(TEXT("CombatSet"));
}

void ASamplePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	{
		// 처음 InitAbilityActorInfo 호출 시, OwnerActor 와 AvatarActor 가 같은 Actor를 가리킴
		// (PlayerState)
		// - OwnerActor는 PlayerState가 맞으나,
		//   AvatarActor는 PlayerController가 소유하는 Pawn이 되어야 함
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	// 아마 당장은 GetPawn이 null이 들어감
	// 차후 재세팅 (HeroComponent)
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

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

	// PawnData의 AbilitySet을 순회하며 ASC에 Ability를 할당(Give)
	for (USampleAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}
