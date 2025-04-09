#include "SampleInventoryItemInstance.h"
#include "SampleInventoryItemDefinition.h"

USampleInventoryItemInstance::USampleInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const USampleInventoryItemFragment* USampleInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<USampleInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		// USampleInventoryItemDefinition 은 모든 멤버 변수가
		// EditDefaultOnly로 선언되어 있으므로, GetDefault로 가져와도 ok 
		// (CDO를 가져오면 원래는 기본값이 BP마다 각각 다르다
		// -> 만들때의 속성으로 EditNewLine과 Instance를 키워드로 주었으며
		// GetDefault 또한 주었으므로 ok)
		// (각자의 독립적인 BP 클래스라서?)
		// 
		// - Fragment 정보는 Instance가 아니라 Def 쪽에 존재
		return GetDefault<USampleInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
