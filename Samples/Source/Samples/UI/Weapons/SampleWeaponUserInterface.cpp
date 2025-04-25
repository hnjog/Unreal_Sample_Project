#include "SampleWeaponUserInterface.h"
#include "Samples/Weapons/SampleWeaponInstance.h"
#include "Samples/Equipment/SampleEquipmentManagerComponent.h"

USampleWeaponUserInterface::USampleWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USampleWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Pawn 가져옴
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		// EquipmentManagerComponent 활용하여, WeaponInstance 가져옴
		if (USampleEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<USampleEquipmentManagerComponent>())
		{
			if (USampleWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<USampleWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					// 새로 업데이트 및 OnWeaponChanged 호출
					USampleWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}
