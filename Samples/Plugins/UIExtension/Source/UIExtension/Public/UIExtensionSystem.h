#pragma once

#include "GameplayTagContainer.h"
#include "Templates/SharedPointer.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIExtensionSystem.generated.h"

// TSharedFromThis : 객체가 자기 자신의 SharedPtr을 얻고 싶을때, 상속받는 헬퍼 클래스
// (std::enable_shared_from_this와 비슷)
// -> 해당 객체가 SharedPtr로 관리되는 것을 암시
// -> 해당 헬퍼 클래스 없이, MakeShared<> 나, (this) 등을 통해 호출 시,
//    별도의 SharedPtr이 생성되기 때문에
//    객체가 두 번 해제되거나, 메모리 누수가 발생할 수 있음
// => 그렇기에 이 헬퍼 클래스를 사용하여 해당 상황을 방지
// 
// ? 그러면 자기 자신을 참조하게 되는 경우 refCount는 어떻게 되는거지?
// => 1 늘어나게 됨
//    그러나, 일반적으로 해당 Scope를 벗어나는 순간 refCount가 줄어듬
//    (단, 멤버 변수 등으로 보관하게 되면 영원히 안 줄어드므로 메모리 누수 확정)
//   그렇기에, Scope 밖으로 반드시 나가는 지역변수를 사용하거나,
//   임의의 시점에 Reset 등을 호출해서 해당 상황을 예방해야 함
struct FUIExtension : TSharedFromThis<FUIExtension>
{
	// UIExtension Widget의 Point Tag
	FGameplayTag ExtensionPointTag;

	// Widget Class를 가지고 있으며, UUIExtensionSubsystem에서 AddReferenceObjects에서 GC를 막음
	UObject* Data = nullptr;

	// 아마 LocalPlayer가 들어올 것으로 설정
	TWeakObjectPtr<UObject> ContextObject;

	int32 Priority = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
	GENERATED_BODY()
public:
	FUIExtensionHandle() {}
	FUIExtensionHandle(UUIExtensionSubsystem* InExtensionSource,const TSharedPtr<FUIExtension>& InDataPtr)
		:ExtensionSource(InExtensionSource),
		DataPtr(InDataPtr)
	{ }

	void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }

	// 아래의 구조체 동작 특성 정의 중 WithIdenticalViaEquality 에 대한 내용
	bool operator==(const FUIExtensionHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionHandle& Other) const { return !operator==(Other); }

public:
	friend class UUIExtensionSubsystem;
	// World마다 다른 UUIExtensionSubsystem(WorldSubsystem 기반)을 들고 있음
	// Subsystem 과 Extension을 매핑하는 용도
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtension> DataPtr;
};

// 구조체 동작 특성 정의
// FUIExtensionHandle 에 대한 특정한 동작을 지원하는지를 엔진에게 알려줌
// WithCopy -> 복사 가능 여부
// WithIdenticalViaEquality -> 동일성 비교 작업(operator==) 여부
//
template<>
struct TStructOpsTypeTraits<FUIExtensionHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	void UnregisterExtension(const FUIExtensionHandle& ExtensionHandle);
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

public:
	// GameplayTag(Slot) <-> FUIExtension(WidgetClass) 매핑
	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;
};