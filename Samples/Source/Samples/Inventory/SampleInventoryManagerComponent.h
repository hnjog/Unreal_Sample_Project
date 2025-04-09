#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SampleInventoryManagerComponent.generated.h"

class USampleInventoryItemInstance;
class USampleInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FSampleInventoryEntry
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<USampleInventoryItemInstance> Instance = nullptr;

};

// Inventory Item 관리 객체
USTRUCT(BlueprintType)
struct FSampleInventoryList
{
	GENERATED_BODY()
public:
	FSampleInventoryList(UActorComponent* InOwnerComponent = nullptr)
		:OwnerComponent(InOwnerComponent)
	{

	}

	USampleInventoryItemInstance* AddEntry(TSubclassOf<USampleInventoryItemDefinition> ItemDef);

public:
	UPROPERTY()
	TArray<FSampleInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

// PlayerController의 Component로서 Inventory 관리
// - 사실 UActorComponent 상속이 아닌 UControllerComponent를 상속받아도? (lyra에선 ActorComponent)
// (UControllerComponent 를 통해 PlayerController를 가져오기 쉬우므로)
// 
// Cosmetic과 유사
// (Dedicate 서버 기반이라서 Network Serilized Array 시스템을 사용하는듯)
// 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLES_API USampleInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USampleInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// InventoryItemDefinition을 통해, InventoryList에 추가하여 관리하고
	// InventoryItemInstance를 반환
	UFUNCTION(BlueprintCallable, Category = Inventory)
	USampleInventoryItemInstance* AddItemDefinition(TSubclassOf<USampleInventoryItemDefinition> ItemDef);

public:
	UPROPERTY()
	FSampleInventoryList InventoryList;
};
