#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SampleEquipmentInstance.generated.h"

// 일반적인 Definition(정의) <-> Instance (저장형) 과는 다르게
// 일부 데이터를 관리해준다
UCLASS(BlueprintType,Blueprintable)
class SAMPLES_API USampleEquipmentInstance : public UObject
{
	GENERATED_BODY()
	
public:
	USampleEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// 어떤 InventroyItemInstance에 의해 활성화되었는지
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	// SampleEquipmentDefinition에 맞게 Spawn된 Actor Instance 들
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
