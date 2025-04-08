#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SampleInventoryItemInstance.generated.h"

class USampleInventoryItemDefinition;

// Definition에서 관리하는 메타데이터를 바탕으로
// 생성하고 관리하는 것이 'Instance'
//
// Definition 을 인벤토리에 저장하게 될때,
// Instance라는 단위로 저장하게 된다
//
UCLASS()
class SAMPLES_API USampleInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	USampleInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// Inventory Item의 인스턴스에는 무엇으로 정의되었는지
	// 그 메타 클래스인 USampleInventoryItemDefinition 를 들고 있음
	UPROPERTY()
	TSubclassOf<USampleInventoryItemDefinition> ItemDef;
};
