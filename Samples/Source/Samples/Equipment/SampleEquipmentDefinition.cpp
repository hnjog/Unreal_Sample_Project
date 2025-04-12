#include "SampleEquipmentDefinition.h"
#include "SampleEquipmentInstance.h"
#include "Samples/AbilitySystem/SampleAbilitySet.h"

USampleEquipmentDefinition::USampleEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// 기본값으로 USampleEquipmentInstance 설정
	InstanceType = USampleEquipmentInstance::StaticClass();
}
