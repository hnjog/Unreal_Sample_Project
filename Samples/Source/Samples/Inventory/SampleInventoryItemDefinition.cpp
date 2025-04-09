#include "SampleInventoryItemDefinition.h"

USampleInventoryItemDefinition::USampleInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const USampleInventoryItemFragment* USampleInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<USampleInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		// Fragments를 순회하며 IsA() 를 통해 해당 클래스를 가지고 있는지 확인
		// 인벤토리의 아이템 종류가 많다면 고민해볼 법 하다?
		// 아이템을 조각으로 나누는 방식으로 모듈화가 진행되어
		// 확장성이 늘어났지만, 성능이 조금 하락할지도?
		// (속성을 얼마나 들고 있느냐에 따라 다를듯)
		// (이 ItemDef가 해당 타입의 조각을 가지고 있는지 체크)
		for (USampleInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
