#include "SampleAbilityCost_ItemTagStack.h"
#include "NativeGameplayTags.h"
#include "Samples/Equipment/SampleGameplayAbility_FromEquipment.h"
#include "Samples/Inventory/SampleInventoryItemInstance.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST,"Ability.ActivateFail.Cost")

USampleAbilityCost_ItemTagStack::USampleAbilityCost_ItemTagStack()
	:Super()
{
	Quantity.SetValue(1.0f);
	FailureTag = TAG_ABILITY_FAIL_COST;
}

bool USampleAbilityCost_ItemTagStack::CheckCost(const USampleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const USampleGameplayAbility_FromEquipment* EquipmentAbility = Cast<const USampleGameplayAbility_FromEquipment>(Ability))
	{
		if (USampleInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
		{
			// 
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			// level에 설정된 값을 가져옴 (없으면 기본값)
			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt32(NumStacksReal);
			// 해당 사용 비용이 합당한지 체크
			const bool bCanApplyCost = ItemInstance->GetStatTagStackCount(Tag) >= NumStacks;

			if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
			{
				// ability 체크 fail인 경우
				OptionalRelevantTags->AddTag(FailureTag);
			}
			return bCanApplyCost;
		}
	}
	return false;
}

void USampleAbilityCost_ItemTagStack::ApplyCost(const USampleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (const USampleGameplayAbility_FromEquipment* EquipmentAbility = Cast<const USampleGameplayAbility_FromEquipment>(Ability))
	{
		if (USampleInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt32(NumStacksReal);

			// 개수만큼 빼준다
			ItemInstance->RemoveStatTagStack(Tag, NumStacks);
		}
	}
}
