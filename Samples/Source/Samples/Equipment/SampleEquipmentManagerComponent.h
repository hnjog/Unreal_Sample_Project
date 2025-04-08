#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SampleEquipmentManagerComponent.generated.h"

class USampleEquipmentDefinition;
class USampleEquipmentInstance;

USTRUCT(BlueprintType)
struct FSampleAppliedEquipmentEntry
{
	GENERATED_BODY()
public:
	// 장착물에 대한 메타 데이터
	UPROPERTY()
	TSubclassOf<USampleEquipmentDefinition> EquipmentDefinition;

	// EquipmentDefinition을 통해 생성된 인스턴스
	UPROPERTY()
	TObjectPtr<USampleEquipmentInstance> Instance = nullptr;

};

// EquimentInstance의 인스턴스는 Entry에서 관리
// - FSampleEquipmentList 는 생성된 객체를 관리
USTRUCT(BlueprintType)
struct FSampleEquipmentList
{
	GENERATED_BODY()
public:
	FSampleEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		:OwnerComponent(InOwnerComponent)
	{

	}

public:

	UPROPERTY()
	TArray<FSampleAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

// Pawn Component 로서 장착물에 대한 관리 
UCLASS()
class SAMPLES_API USampleEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	USampleEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY()
	FSampleEquipmentList EquipmentList;
};
