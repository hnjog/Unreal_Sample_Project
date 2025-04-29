#include "SampleInventoryFragment_SetStats.h"
#include "SampleInventoryItemInstance.h"

void USampleInventoryFragment_SetStats::OnInstanceCreated(USampleInventoryItemInstance* Instance) const
{
	for (const auto& InitialItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(InitialItemStat.Key, InitialItemStat.Value);
	}
}
