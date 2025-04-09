#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "SampleQuickBarComponent.generated.h"

class USampleInventoryItemInstance;
class USampleEquipmentInstance;

// HUD의 QuickBar (MMO의 Shortcut HUD)
// 아이템 빠르게 사용하는 그것(아니면 FPS의 1번 주무기 이런것)
// 
// ControllerComponent로서, PlayerController에 의한 조작계 중 하나
// - HUD(Slate)와 Inventory/Equipment(Gameplay) 의 다리(Bridge) 역할을 하는 Component
// - UI 할때 다시 수정 예정
UCLASS()
class SAMPLES_API USampleQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	USampleQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, USampleInventoryItemInstance* Item);
public:
	// HUD QuickBar Slot 갯수
		UPROPERTY()
	int32 NumSlots = 3;

	// HUD QuickBar Slot 리스트
	UPROPERTY()
	TArray<TObjectPtr<USampleInventoryItemInstance>> Slots;

	// 현재 활성화된 Slot Index (Lyra는 딱 한개만 Slot이 활성화?)
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	// 현재 장착한 장비 정보
	UPROPERTY()
	TObjectPtr<USampleEquipmentInstance> EquippedItem;
};
