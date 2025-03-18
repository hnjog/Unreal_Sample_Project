#include "SampleHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "../SampleLogChannels.h"
#include"../SampleGameplayTags.h"
#include"SamplePawnExtensionComponent.h"
#include"../Player/SamplePlayerState.h"
#include"SamplePawnData.h"
#include"../Camera/SampleCameraMode.h"
#include"../Camera/SampleCameraComponent.h"

const FName USampleHeroComponent::NAME_ActorFeatureName("Hero");

USampleHeroComponent::USampleHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Tick을 꺼준다
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

}

void USampleHeroComponent::OnRegister()
{
	Super::OnRegister();
	// PawnExtensionComponent 와 동일
	// Actor에 제대로 붙었는지 확인하기
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogSample, Error, TEXT("this component has been added to a BP Whose base class is not a Pawn!"));
			return;
		}
	}

	// InitState 사용을 위한 등록 진행
	RegisterInitStateFeature();
}

void USampleHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtension의 모든 상태변화를 받겠다고 구독
	BindOnActorInitStateChanged(USamplePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// 상태 변화 감지
	ensure(TryToChangeInitState(FSampleGameplayTags::Get().InitState_Spawned));

	// 자신의 초기화 호출
	CheckDefaultInitialization();
}

void USampleHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void USampleHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FSampleGameplayTags& InitTags = FSampleGameplayTags::Get();
	if (Params.FeatureName == USamplePawnExtensionComponent::NAME_ActorFeatureName)
	{
		// PawnExtension에서 들어온 것인지 체크하고
		// 해당 컴포넌트의 상태를 체크하고
		// 그 때, 자신의 CheckDefaultInitialization를 호출
		// 일종의 보험(PawnExtension에서 호출을 하고 있는 상황이지만, 순서가 꼬이는 경우 등을 대비한 것으로 보인다)
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
	
}

bool USampleHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FSampleGameplayTags& InitTags = FSampleGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ASamplePlayerState* SamplePS = GetPlayerState<ASamplePlayerState>();

	// Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// PlayerState는 Pawn에 possess가 호출된 이후에 정상적으로 호출이 가능하기에 여기서 멈춤
		if (!SamplePS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		return SamplePS && Manager->HasFeatureReachedInitState(Pawn, USamplePawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void USampleHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FSampleGameplayTags& InitTags = FSampleGameplayTags::Get();

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ASamplePlayerState* SamplePS = GetPlayerState<ASamplePlayerState>();
		if (!ensure(Pawn && SamplePS))
			return;

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const USamplePawnData* PawnData = nullptr;
		if (USamplePawnExtensionComponent* PawnExtComp = USamplePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<USamplePawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			// 현재 SampleCharacter에 Attach 된 CameraComponent를 찾는다
			if (USampleCameraComponent* CameraComponent = USampleCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void USampleHeroComponent::CheckDefaultInitialization()
{
	// PawnExtension의 경우는 다른 Component들에게 전부 이 함수를 호출하라 하지만
	// Hero는 딱히 다른 컴포넌트에 영향을 줄 필욘 없음
	// -> 이 컴포넌트가 다른 컴포넌트의 존재를 몰라도 되기에 종속성이 없어진다
	// Pawn도 manager에게 요청하는 것이지, 딱히 다른 컴포넌트를 직접 호출하진 않는다
	const FSampleGameplayTags& InitTags = FSampleGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

PRAGMA_DISABLE_OPTIMIZATION
TSubclassOf<USampleCameraMode> USampleHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (Pawn == nullptr)
		return nullptr;

	if (USamplePawnExtensionComponent* PawnExtComp = USamplePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const USamplePawnData* PawnData = PawnExtComp->GetPawnData<USamplePawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION