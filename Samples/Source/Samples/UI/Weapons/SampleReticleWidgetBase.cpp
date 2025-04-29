#include "SampleReticleWidgetBase.h"
#include "Samples/Weapons/SampleWeaponInstance.h"
#include "Samples/Inventory/SampleInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SampleReticleWidgetBase)

USampleReticleWidgetBase::USampleReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void USampleReticleWidgetBase::InitializeFromWeapon(USampleWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;
	if (WeaponInstance)
	{
		InventoryInstance = Cast<USampleInventoryItemInstance>(WeaponInstance->GetInstigator());
	}

	OnWeaponInitialized();
}
