#include "SampleQuickBarComponent.h"
#include"SampleEquipmentInstance.h"
#include"SampleEquipmentDefinition.h"
#include"SampleEquipmentManagerComponent.h"
#include"Samples/Inventory/SampleInventoryItemInstance.h"
#include"Samples/Inventory/SampleInventoryFragment_EquippableItem.h"

USampleQuickBarComponent::USampleQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USampleQuickBarComponent::BeginPlay()
{
	// 미리 할당
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

USampleEquipmentManagerComponent* USampleQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<USampleEquipmentManagerComponent>();
		}
	}

	return nullptr;
}

void USampleQuickBarComponent::UnequipItemInSlot()
{
	// QuickBar는 Controller에 붙어있는 Component지만
	// USampleEquipmentManagerComponent는 Controller가 소유(possess)하고 있는
	// Pawn의 Component
	if (USampleEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem) // 장착 아이템 존재
		{
			// EquipmentManager 를 통해 장착 해제 시킴
			EquipmentManager->UnequipItem(EquippedItem);

			EquippedItem = nullptr;
		}
	}
}

void USampleQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	// 현재 활성화된 ActiveSlotIndex를 활용하여 활성화된 InventoryIteminstance를 찾는다
	if (USampleInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// Slot Item을 통해 (InventoryItemInstance)
		// InventoryFragment_EquippableItem의 Fragment를 찾음
		// - 찾는 것이 실패했다면 장착할 수 없는 Inventory Item
		if (const USampleInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<USampleInventoryFragment_EquippableItem>())
		{
			// EquippableItem에서 EquipmentDefinition을 찾음
			TSubclassOf<USampleEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				// EquipManager를 통해 장착
				if (USampleEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);

					if (EquippedItem)
					{
						// EquippedItem에는 앞서 보았던 Instiagor로 Slot을 대상으로 넣는다
						EquippedItem->Instigator = SlotItem;
					}
				}

			}
		}
	}

}

void USampleQuickBarComponent::AddItemToSlot(int32 SlotIndex, USampleInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		// Slots을 Index에 바로 넣음
		// Size는 BeginPlay 등에서 조정할 예정
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void USampleQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if ((Slots.IsValidIndex(NewIndex)) && (ActiveSlotIndex != NewIndex))
	{
		// UnequipItem/ EquipItem을 통해, NewIndex를 통해 할당된 Item을 장착 및 업데이트
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}
