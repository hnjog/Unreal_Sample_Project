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

	// Definition은 정의, Instance는 생성 이후의 각종 세팅을 담당
	// 그렇기에 Definition의 하위 속성들에게 자신이 생성되었음을 알린다
	// (지금은 아이템이니, 장착, 스탯 등에게 실제 생성된 현재 Instance를 전달)
	for (const USampleInventoryItemFragment* Fragment : GetDefault<USampleInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
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
