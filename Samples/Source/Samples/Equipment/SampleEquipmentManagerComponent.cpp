#include "SampleEquipmentManagerComponent.h"
#include"SampleEquipmentDefinition.h"
#include"SampleEquipmentInstance.h"

USampleEquipmentManagerComponent::USampleEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	EquipmentList(this) // UPawnC -> UGameFramewC -> UAC
{
}
