#include "SampleEquipmentInstance.h"

USampleEquipmentInstance::USampleEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

APawn* USampleEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void USampleEquipmentInstance::OnEquipped()
{
	// 이러한 효과는 BP에서 작성하도록
	// 애니메이션 로직은 BP가 편하기에 이런듯?
	K2_OnEquipped();
}

void USampleEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
