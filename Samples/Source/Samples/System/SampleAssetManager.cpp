// 민감한 폴더 이름을 사용하는 경우는 경로를 확실히 하자
#include "SampleAssetManager.h"
#include "Samples/SampleLogChannels.h"
#include"../SampleGameplayTags.h"

USampleAssetManager::USampleAssetManager()
{
}

// 에셋 매니저 : singleton
// -> 유일성을 보장하는 패턴
USampleAssetManager& USampleAssetManager::Get()
{
	check(GEngine);

	// 이 클래스가 오버라이드 된 클래스이기에, GEngine에 AssetManager가 존재
	if (USampleAssetManager* Singleton = Cast<USampleAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogSample, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be SampleAssetManager"));

	// crash가 나기에 도달할 수 없는 상황이지만 컴파일을 위해
	return *NewObject<USampleAssetManager>();
}

PRAGMA_DISABLE_OPTIMIZATION
void USampleAssetManager::StartInitialLoading()
{
	// 생성 시점 : 에디터가 켜지기 전 시점 -> Thread-safe 하지 않음
	Super::StartInitialLoading();

	// 태그 싱글톤 호출
	// 해당 시점엔 Editor가 켜지기 전 시점
	FSampleGameplayTags::InitializeNativeTags();
}
PRAGMA_ENABLE_OPTIMIZATION

bool USampleAssetManager::ShouldLogAssetLoads()
{
	// commandLine을 통하여 command의 인자를 가져올 수 있음
	// 언리얼 엔진에서도 - 이후 인자를 입력할 수 있고, FCommandLine을 통해 그 인자를 가져올 수 있음
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

// synchronous load asset을 불필요하게 호출하는 경우를 확인하기 위함
// (동기 로딩이 호출되면 게임이 끊기거나 프레임 드랍이 발생하므로 그러한 경우를 체크하기 위한 용)
// 동기식 에셋 로드
// 동기 : 완전히 끝날때까지 기다린 후, 다음 작업 진행 (게임 시작 시, 대량 로딩) -> 빠름, 직관적
// 비동기 : 일정 시간 진행 후, 다른 스레드 등을 통하여 타 작업을 진행 (인게임 중 로딩) -> 유저 경험
UObject* USampleAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// FSoftObjectPath : 실제로 들고 있는 것이 아닌, 해당 에셋의 파일 경로를 들고 있는 용도

	if (AssetPath.IsValid())
	{
		//FScopeLogTime 확인
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서, 초단위로 로깅 진행
			// 동기 로딩의 시간 체크
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 아래의 두 함수는 정말로 로딩이 안되어 있는 경우에만 동적 로딩이 호출됨
		// 캐싱이 되어있다면 그걸 그대로 가져옴

		// 여기서 두가지 분기:
		// 1. AssetManager가 있으면, AssetManager의 StreamableManager를 통해 정적 로딩
		// 2. 아니면, FSoftObjectPath를 통해 바로 정적 로딩
		if (UAssetManager::IsValid())
		{
			// 진짜 로딩을 진행
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// if asset manager is not ready, use LoadObject()
		// - 슥 보면, StaticLoadObject가 보인다: 
		// - 참고로, 항상 StaticLoadObject하기 전에 StaticFindObject를 통해 확인하고 실패하면 진짜 로딩함
		// 매우 느린 로딩 방식(어쩔수 없는 경우에만 사용)
		return AssetPath.TryLoad();
	}
	return nullptr;
}

void USampleAssetManager::AddLoadedAsset(const UObject* Asset)
{
	// 주어진 조건이 항상 참인지 확인할때 사용하는 매크로 (디버깅 모드)
	// Asset null 체크
	if (ensureAlways(Asset))
	{
		// 임계영역 잠금 관리용 클래스
		// 생성될때 lock 획득, 범위 종료 시 잠금 해제 (Thread-Safe)
		// SyncObject가 임계영역이므로, 그것에 Lock을 걸고
		// 안전하게 Set에 Add
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
