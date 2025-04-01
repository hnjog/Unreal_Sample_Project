#include "SampleExperienceManagerComponent.h"
#include "../System/SampleAssetManager.h"
#include "../GameModes/SampleExperienceDefinition.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"
#include"Samples/GameModes/SampleExperienceActionSet.h"
#include "GameFeatureAction.h"

void USampleExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		// 요청하는 함수에게 이미 로딩이 되었으므로
		// 현재 Experience를 인자로 건네주며 실행시킨다(callback)
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		// movetemp 를 통해 rvalue Reference 위치를 이동 시킴
		// Delegate 리스트에 해당 함수를 등록
		//
		// Delegate 객체는 내부적으로 필요한 변수를 메모리 할당해놓음
		// (함수 인자 등, 여러 상태를 저장하는 변수
		// 
		// ex)
		// TArray<int> a = {1,2,3,4}
		// delegate_type delegate = [a](){return a.num;}
		// -> a는 delegate_type 내부에 new로 할당이 된다
		//  -> 복사 비용을 낮추기 위하여 Move를 통하여 전달
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void USampleExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	USampleAssetManager& AssetManager = USampleAssetManager::Get();

	TSubclassOf<USampleExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		// B_ 블루프린트가 load 될 것
		AssetClass = Cast<UClass>(AssetPath.TryLoad()); // 동기식 load
	}

	// CDO : Class Default Object
	// 특정한 클래스의 기본 인스턴스
	// -> 해당 클래스의 기본 속성값을 가진 객체
	// (CDO의 값을 변경하는 경우, 차후 생성되는 인스턴스의 기본값이 변경됨)
	// 
	// '기본값의 모음'이자 객체화된 인스턴스
	// 
	// CDO 를 가져오는 이유?
	// 직렬화 하는 과정에서 Delta Serialization 을 지원하기 위함
	// -> 직렬화는 데이터를 저장하는 방식
	// -> Delta 직렬화는 데이터의 '변경 사항'만을 기록하여 데이터를 저장
	// 
	// CDO를 통해 기본값과 비교하여 바뀐 값만 저장하는 방식을 통해
	// 메모리 절약 및 네트워크 서버 성능 개선 가능
	// (바뀐 부분만 저장 및 전송)
	// 
	// CDO를 마음대로 수정하는것은 위험하지 않나?
	//
	const USampleExperienceDefinition* Experience = GetDefault<USampleExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// CDO로 CurrentExperience를 설정
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

// 최적화 비활성화
// -> 최적화로 인한 예상치 못한 동작을 방지 (디버깅, 성능 테스트 등에서 활용)
PRAGMA_DISABLE_OPTIMIZATION
void USampleExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ESampleExperienceLoadState::Unloaded);

	// Load 시작
	LoadState = ESampleExperienceLoadState::Loading;
	USampleAssetManager& AssetManager = USampleAssetManager::Get();

	// ServerSetCurrentExperience에선 ExperienceId를 넘겨주었는데, 여기선 CDO를 활용하여
	// GetPrimaryAssetId를 로딩할 대상으로 넣는다

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// load Assets associated with the experience
	// GameFeature 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가
	// - Bundle : 우리가 로딩할 에셋의 카테고리 이름이라 생각하면 됨
	//
	TArray<FName> BundlesToLoad;
	{
		// OwnerNetMode가 NM_Standalone이면, Client/Server 둘 다 로딩에 추가
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer); // 서버가 아니면 클라
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);	// 클라가 아니면 서버

		// Lyra는 멀티 서버 기반의 프로젝트
		// 클라에서만 필요한 것이,
		// 서버에서만 필요한 것이 존재하기에
		// BundledsToLoad에 각각 상황에 맞게 태그를 넣어
		// 적절한 PrimaryAsset을 Load시킨다
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient); // TEXT("Client") 랑 동일
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer); // TEXT("Server") 랑 동일
		}
	}

	// Delegate 생성
	// load 완료시 호출할 녀석 달아줌
	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// 지금은 B_SampleDefaultExperience를 로딩하는 용도
	// 비동기 로딩 진행
	// 아깐 TryLoad()를 사용했지만
	// 여긴 CDO와 ChangeBundleStateForPrimaryAssets 등을 사용하여 비동기 로드를 진행?
	// -> 원래 lyra 프로젝트는 대규모 에셋을 관리하는 시스템을 이용
	// 
	// BundlesToLoad에는 현재 Server와 Client 이름이 들어가 있음
	// 
	// USampleExperienceDefinition에 매우 많은 데이터가 들어가 있다고 생각했을때,
	// (ex : 직업 세팅이나 아이템 세팅 등등)
	// -> 전부 로딩하고 싶지 않고, 만약 특정한 존재만 로딩을 하고 싶다면?
	// (ex : 직업 세팅 중 '전사'만 로딩하고 싶음)
	// ChangeBundleStateForPrimaryAssets
	// -> 특정한 BundleAssetList.Array를 통하여 Load할 데이터만
	//	  BundleToLoad에 집어넣어준다
	//
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출하자:
		// - 아래의 함수를 확인해보자:
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		// 완료되지 않은 경우는 Delegate에 등록시킨다
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		// 문제가 되는 경우(예외처리)
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				// ExecuteIfBound : delegate가 무사한 경우, 실행한다
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// FrameNumber를 주목해서 보자
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}
PRAGMA_ENABLE_OPTIMIZATION

// Experience의 Load 완료 후, GameFeature Plugin을 로드
void USampleExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;
	check(LoadState == ESampleExperienceLoadState::Loading);
	check(CurrentExperience);

	// 이전에 활성화된 GameFeature Plugin의 URL을 초기화
	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			// FeaturePluginList를 순회하며, PluginURL을 ExperienceManagerComponent의 GameFeaturePluginURLs에 추가
			for (const FString& PluginName : FeaturePluginList)
			{
				FString PluginURL;
				// UGameFeaturesSubsystem : 엔진의 서브시스템을 상속받아 만든 것
				// GetPluginURLByName : Plugin의 이름을 통해 그 Plugin의 URL을 가져오는 함수
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
				{
					This->GameFeaturePluginURLs.AddUnique(PluginURL);
				}
			}
		};

	// GameFeaturesToEnable에 있는 Plugin만 일단 활성화할 GameFeature Plugin 후보로 등록
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	// GameFeaturePluginURLs에 등록된 Plugin들을 로딩 및 활성화
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = ESampleExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			// 매 Plugin이 로딩 및 활성화 이후, OnGameFeaturePluginLoadComplete 콜백 함수 등록
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		// GameFeature Plugin이 없는 경우, 바로 OnExperienceFullLoadCompleted 호출
		// FStreamableDelegateDelayHelper에 의해 불림
		OnExperienceFullLoadCompleted();
	}
}

void USampleExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// 매 GameFeature Plugin 로딩될 때마다, 이 함수가 콜백
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		// 모든 로딩이 다 끝난 이후 호출
		OnExperienceFullLoadCompleted();
	}
}

void USampleExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ESampleExperienceLoadState::Loaded);

	// GameFeature Plugin 로딩과 활성화 이후, GameFeature Action을 활성화
	{
		LoadState = ESampleExperienceLoadState::ExcutingActions;

		// GameFeature Action을 활성화를 위한 Context
		FGameFeatureActivatingContext Context;
		{
			// GameFeatureAction을 관리하는 것은 GameFeatureSystem(EngineSubsystem 기반)
			// 엔진 서브 시스템은 엔진 자체가 종료될때까지 소멸되지 않음
			// (PI를 만들더라도 계속 남아있음)
			// 
			// 그런데 Action은 엔진 기반의 SubSystem에서 관리하므로,
			// 어떠한 World에서 Action을 활성화할지를 명시해주어야 함
			// (안그러면 죄다 Bind 해줘야 하기에)
			// 
			// 월드의 핸들을 세팅
			// ContextHandle를 통하여 월드를 고유하게 식별
			const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWorldContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			}
		}

		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
			{
				for (UGameFeatureAction* Action : ActionList)
				{
					// 명시적으로 GameFeatureAction에 대해 Register -> Loading -> Activating 순으로 호출
					if (Action)
					{
						Action->OnGameFeatureRegistering(); // 등록
						Action->OnGameFeatureLoading(); // 로드
						Action->OnGameFeatureActivating(Context); // 활성화 (이때 월드 컨텍스트를 전달)
					}
				}
			};

		// 1. Experience의 Actions
		ActivateListOfActions(CurrentExperience->Actions);

		// 2. Experience의 ActionSet
		for (const TObjectPtr<USampleExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}

	LoadState = ESampleExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const USampleExperienceDefinition* USampleExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ESampleExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
