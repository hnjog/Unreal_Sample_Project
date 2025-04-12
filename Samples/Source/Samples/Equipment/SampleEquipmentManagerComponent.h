#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Samples/AbilitySystem/SampleAbilitySet.h"
#include "SampleEquipmentManagerComponent.generated.h"

class USampleEquipmentDefinition;
class USampleEquipmentInstance;

// 장착이 적용된 요소들
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

	// 무기에 할당된 허용가능한 GameplayAbility
	UPROPERTY()
	FSampleAbilitySet_GrantedHandles GrantedHandles;
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

	USampleEquipmentInstance* AddEntry(TSubclassOf<USampleEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(USampleEquipmentInstance* Instance);

	USampleAbilitySystemComponent* GetAbilitySystemComponent() const;

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

	USampleEquipmentInstance* EquipItem(TSubclassOf<USampleEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(USampleEquipmentInstance* ItemInstance);

public:
	UPROPERTY()
	FSampleEquipmentList EquipmentList;
};
