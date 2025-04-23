#include "SampleExperienceDefinition.h"
#include"GameFeatureAction.h"
#include"SampleExperienceActionSet.h"

USampleExperienceDefinition::USampleExperienceDefinition(const FObjectInitializer& ObjectInitalizer)
	: Super(ObjectInitalizer)
{

}

#if WITH_EDITORONLY_DATA
// primaryDataAsset에서 만든 함수
// -> 동적으로 번들을 추가할 수 있도록 한다
void USampleExperienceDefinition::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}

}
#endif