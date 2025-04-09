#include "AsyncAction_ExperienceReady.h"
#include "SampleExperienceManagerComponent.h"

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* Action = nullptr;
	// World Context 기반으로 World를 캐싱해둔다
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// 자신에 대한 Class 를 만들고
		// 그것을 포인터로 넘겨줌
		Action = NewObject<UAsyncAction_ExperienceReady>();
		Action->WorldPtr = World;
		Action->RegisterWithGameInstance(World);
	}
	return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
	// 캐싱된 월드를 가져와서 체크
	if (UWorld* World = WorldPtr.Get())
	{
		// GameState가 이미 World에 준비되어 있으면,
		// Step1을 스킵하고 바로 Step2를 진행
		if (AGameStateBase* GameState = World->GetGameState())
		{
			Step2_ListenToExperienceLoading(GameState);
		}
		else
		{
			// 아직 준비되어 있지 않으면
			// UWorld::GameStateSetEvent에 Step1_HandleGameStateSet을 Delegate로 바인딩
			World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
		}
	}
	else
	{
		// World가 없다..?
		// World가 바뀌거나 존재하지 않는 상황
		// 
		// 현재 AsyncAction 대상이었던 World가 더이상 Valid하지 않으므로, 제거 진행
		// - 내부에서 GameInstance에서 레퍼런스 대상으로 제거시킴
		// (UAsyncAction의 캐싱 위치가 GameInstance)
		// (게임 인스턴스에서 자기 자신을 레퍼런스를 끊는다면
		// ref count가 0이 되어 GC가 수거해가게 됨)
		// 
		SetReadyToDestroy();
	}
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* World = WorldPtr.Get())
	{
		// 이 클래스가 등록한 Delegate 함수 제거
		World->GameStateSetEvent.RemoveAll(this);
	}

	Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	check(GameState);
	// Expierence 로딩 상태 정보를
	// GameState에 붙어있는 ExperienceManagerComponent에서 가져올 수 있음
	// (GameStateSetEvent에 Delegate로 바인딩시킨 이유)
	USampleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<USampleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// 이미 Experience가 준비되었다면, Step3를 스킵하고 Step4로
	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		UWorld* World = GameState->GetWorld();
		check(World);

		// 이 함수가 불리는 시점은 일반적으로는 inven,equip 등이 다 세팅되어 있는
		// possess 시점
		// 
		// 캐릭터의 Mesh는 PawnComponent_CharacterParts에 의해서 결정
		// 
		// 정확하게 말하면 ControllerComponent_CharacterParts에서 설정된 
		// Character Part의 정보에 따라서 Mesh를 선택해서 결정
		// 
		// ControllerComponent_CharacterParts가 설정하는 시점에는
		// Pawn이 Possess되지 않기 때문에 UPawnComponent_CharacterParts을 
		// 호출할 수 없어서 결론적으로 Mesh 설정이 불가능
		// 
		// (Mesh 장착 이후, 아이템 장착)
		// (아이템 장착시, Mesh의 AnimLayer를 바꾸기에)
		// 
		// 그래서 ControllerComponent_CharacterParts는 
		// OnPossessedPawnChanged 델리게이트를 통해 Possess되는 시점에 발동하도록 설정하는데
		// 여기서 문제가 발생
		// (근데 OnPossessedPawnChanged를 Cotroller 에서 등록해놓았음 -> Part를 통해 Mesh 설정)
		// 
		// 왜냐면 Possess 함수는 내부적으로 OnPossessedPawnChanged 델리게이트를 Possess 가장 마지막 단계로 호출하기에
		// 현재 캐릭터의 PossessedBy를 먼저 호출
		// (그러면 결국 일반적인 호출 상황에선 아이템 장착이 Mesh 설정보다 빠를 수밖에 없음)
		// 
		// 그렇게 되면 Mesh가 설정이 되어 있지 않아서 아이템 장착할 때 AnimLayer 그리고 장비 아이템 장착이 불가능해진다
		// 
		// 그래서 해결 방법으로 현 프레임이 아닌 안전하게 다음 프레임에서 처리하도록 해서 
		// OnPossessedPawnChanged가 먼저 호출 되도록 유도
		// 
		// 아미 Experience가 준비되었다고 해도, 
		// 아직 준비되지 않은 상태가 있을 수도 있으니 (혹은 과정중이라던가?) 
		// 그냥 다음 Tick에 실행되도록 Timer를 바인딩시킨다
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::Step4_BroadcastReady));
	}
	else
	{
		// 준비가 안되었다면, OnExperienceLoaded에 바인딩시킨다 (후일 로딩 끝나면 Step3부터 실행이 될 것이다)
		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::Step3_HandleExperienceLoaded));
	}
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const USampleExperienceDefinition* CurrentExperience)
{
	// 지금 따로 ExperienceDefinition에 대해 처리할 로직은 없음
	Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	// 바인딩된 BP 혹은 UFUNCTION을 호출해준다
	OnReady.Broadcast();
	SetReadyToDestroy();
}