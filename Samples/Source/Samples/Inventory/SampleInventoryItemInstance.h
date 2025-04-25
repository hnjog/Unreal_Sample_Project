#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SampleInventoryItemInstance.generated.h"

class USampleInventoryItemDefinition;
class USampleInventoryItemFragment;

// Definition에서 관리하는 메타데이터를 바탕으로
// 생성하고 관리하는 것이 'Instance'
//
// Definition 을 인벤토리에 저장하게 될때,
// Instance라는 단위로 저장하게 된다
//
UCLASS(BlueprintType)
class SAMPLES_API USampleInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	USampleInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 아래의 템플릿 클래스와 함께
	// 특정 '클래스'를 받아 그 클래스 객체를 반환하는 방식
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const USampleInventoryItemFragment* FindFragmentByClass(TSubclassOf<USampleInventoryItemFragment> FragmentClass) const;

	template<typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

public:
	// Inventory Item의 인스턴스에는 무엇으로 정의되었는지
	// 그 메타 클래스인 USampleInventoryItemDefinition 를 들고 있음
	UPROPERTY()
	TSubclassOf<USampleInventoryItemDefinition> ItemDef;
};
