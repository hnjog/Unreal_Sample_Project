#include "SampleQuickBarComponent.h"
#include"Samples/Inventory/SampleInventoryItemInstance.h"
#include"SampleEquipmentInstance.h"

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
