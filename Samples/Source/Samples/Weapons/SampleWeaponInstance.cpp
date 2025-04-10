#include "SampleWeaponInstance.h"

USampleWeaponInstance::USampleWeaponInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

TSubclassOf<UAnimInstance> USampleWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FSampleAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
