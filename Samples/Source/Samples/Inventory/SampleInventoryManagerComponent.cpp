#include "SampleInventoryManagerComponent.h"
#include "SampleInventoryItemInstance.h"
#include "SampleInventoryItemDefinition.h"

USampleInventoryItemInstance* FSampleInventoryList::AddEntry(TSubclassOf<USampleInventoryItemDefinition> ItemDef)
{
	USampleInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	// OwnerComponent : USampleInventoryManagerComponent
	//  OwnerComponent->GetOwner() : PlayerController
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FSampleInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<USampleInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	Result = NewEntry.Instance;

	return Result;
}

USampleInventoryManagerComponent::USampleInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	InventoryList(this) // ActorComponent 인 자신을 전달
{

}

USampleInventoryItemInstance* USampleInventoryManagerComponent::AddItemDefinition(TSubclassOf<USampleInventoryItemDefinition> ItemDef)
{
	USampleInventoryItemInstance* Result = nullptr;

	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}
