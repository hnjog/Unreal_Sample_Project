#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SampleAssetManager.generated.h"

UCLASS()
class SAMPLES_API USampleAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	USampleAssetManager();
	// Scan?
	// - FSoftObjectPath 처럼 무언가가 '존재'하는 위치를 저장하듯
	// 에셋 매니저도 '특정한 에셋이 존재'한다는 것을 알기 위한 개념

	static USampleAssetManager& Get();

	virtual void StartInitialLoading() final;

	// FScopeLogTime의 사용 여부 판단을 위한 함수
	static bool ShouldLogAssetLoads();

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	// GetAsset : 정적 로딩으로 BP Class 와 Object를 로딩한다

	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// [Thread-Safe] 메모리에 로딩된 에셋 캐싱
	void AddLoadedAsset(const UObject* Asset);

	// GC의 대상
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets; // 캐싱을 위한 일종의 장치 (여기 담아둠으로서 메모리에 상주시킴)

	// Object 단위의 Locking
	FCriticalSection SyncObject;
};

// Asset : 게임 내의 모든 리소스 (UObject 상속)
// 완성된 '객체'를 저장하기 위한 단위
// 
// staticclass : 메타데이터(리플렉션)
// 특정한 클래스의 UClass를 반환하는 메서드 (클래스의 메타 데이터에 접근하는 방법)
// (리플렉션 : 런타임에 클래스,객체의 메타데이터에 접근하여 관련 정보를 동적으로 조회) -> 런타임 중 타입 검사, 속성 접근, 인스턴스 생성 등
// UCLASS : 클래스 정의, 해당 클래스를 리플렉션에 포함시킴 (UClass를 구현하여 리플렉션의 다양한 정보를 저장, Field 등)
// USTRUCT : 구조체를 정의
// UPROPERTY : 멤버 변수를 리플렉션에 포함 (변수 노출 및 편집기에서 접근 가능하도록)
// 
// IsA<> : 객체 타입 확인
// GetProperty() : 속성 접근, 동적으로 읽거나 수정
// NewObject<> 함수를 통해 동적 객체 생성
// 
// TSubclassof : 특정 클래스의 서브클래스를 타입정보
// 특정 클래스의 서브 클래스를 참조할 수 있는 템플릿 클래스
// (서브 클래스 : 자식 클래스)
// (TSubclassof<> 를 통해 특정 클래스만 허용하도록 제한 가능 - 안전한 참조)
// 
// CDO(Class Default Object) : 클래스의 기본 인스턴스
// 클래스가 처음 로드될 때 자동 생성 (해당 클래스의 모든 인스턴스가 공통으로 사용하는 기본값 제공)
//
// 언리얼은 각종 매크로들을 이용하여 리플렉션을 구현한 상황이다
// + 빌드가 오래걸리는 이유 중 하나는 각 클래스들을 Uclass 버전으로 빌드하기에
template <typename AssetType>
AssetType* USampleAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepsInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		// 로딩이 되어있다? -> 바로 가져옴
		// 로딩이 안되어 있다 -> Null
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			// 동기 에셋 로드
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		// 메모리에 남겨두는지 확인
		if (LoadedAsset && bKeepsInMemory)
		{
			// 여기서 AddLoadAsset은 메모리에 상주하기 위한 장치라고 생각하면 됨:
			// - 한번 등록되면 직접 내리지 않는한 Unload가 되지 않음 (== 캐싱)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

// 반환을 클래스 타입으로
template <typename AssetType>
TSubclassOf<AssetType> USampleAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	// AssetType 에 해당하는 녀석들만 참조로 가지려 함
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		// 적절한 클래스가 매칭되었는지를 확인
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
