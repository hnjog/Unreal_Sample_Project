#include "SampleInventoryManagerComponent.h"
#include "SampleInventoryItemInstance.h"

USampleInventoryManagerComponent::USampleInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	InventoryList(this) // ActorComponent 인 자신을 전달
{

}
