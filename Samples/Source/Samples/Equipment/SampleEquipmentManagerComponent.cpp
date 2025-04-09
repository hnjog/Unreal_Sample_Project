#include "SampleEquipmentManagerComponent.h"
#include"SampleEquipmentDefinition.h"
#include"SampleEquipmentInstance.h"

USampleEquipmentInstance* FSampleEquipmentList::AddEntry(TSubclassOf<USampleEquipmentDefinition> EquipmentDefinition)
{
	USampleEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	// EquipmentDefinition의 멤버 변수들은 EditDefaultsOnly로 정의되어 있어 
	// GetDefault로 들고 와도 우리에게 필요한 것들이 모두 들어있다
	const USampleEquipmentDefinition* EquipmentCDO = GetDefault<USampleEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<USampleEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		// 없는 경우, Crash를 내는 방식도 있긴 하나
		// lyra에선 기본의 staticclass로
		InstanceType = USampleEquipmentInstance::StaticClass();
	}

	// Entries에 추가해주자
	FSampleAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<USampleEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	// ActorsToSpawn을 통해,EquipmentInstance에 Actor들을 인스턴스화
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FSampleEquipmentList::RemoveEntry(USampleEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; EntryIt++)
	{
		FSampleAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			// Actor 제거 작업 및 Array에서 제거
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

USampleEquipmentManagerComponent::USampleEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	EquipmentList(this) // UPawnC -> UGameFramewC -> UAC
{
}

USampleEquipmentInstance* USampleEquipmentManagerComponent::EquipItem(TSubclassOf<USampleEquipmentDefinition> EquipmentDefinition)
{
	USampleEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		// Actor Spawn
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}

	return Result;
}

void USampleEquipmentManagerComponent::UnequipItem(USampleEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		// 해당 함수로 BP의 Event노드 호출
		ItemInstance->OnUnequipped();

		//EquipmentList에서 제거 (Actor Instance를 제거)
		EquipmentList.RemoveEntry(ItemInstance);
	}
}
