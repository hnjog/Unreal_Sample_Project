#include "SampleExperienceActionSet.h"
#include"GameFeatureAction.h"


USampleExperienceActionSet::USampleExperienceActionSet()
{

}

#if WITH_EDITORONLY_DATA
// primaryDataAsset에서 만든 함수
// -> 동적으로 번들을 추가할 수 있도록 한다
// 실제 게임을 빌드할때, 이 함수가 호출되어
// 에셋 번들 데이터를 최신 상태로 갱신하기에
// 실제 게임 상황에선 이 함수가 호출될 필요는 없음
// 
// Super::UpdateAssetBundleData()
// -> UAssetManager의 InitializeAssetBundlesFromMetadata 를 통하여
// 해당 Action과 Actionset 내부의 프로퍼티들을 가져옴
// 
// GameFeatureAction은 UObject이기에
// InitializeAssetBundlesFromMetadata 를 통해 재귀적으로 내부 프로퍼티를 가져옴
// 
// 우리가 자료형 구조체에 USTRUCT를 붙였다면 해당하는 Data들도 이 떄 가져오게 됨
// (struct type인 것을 확인)
// (+ 이후 내부에서 UPROPERTY를 확인하고 가져옴)
// (이 때 AssetBundlesName을 확인
//  ex : meta = (AssetBundles = "Client,UI"))
// 이러한 방식으로 asset bundle을 체크하고 해당 name이 있다면 가져오고 아니면 pass
//
void USampleExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			// 번들 데이터를 추가함으로서
			// 내부 에셋들의 관계와 로딩 우선순위, 의존성 등에 대한 정보를 추가

			// 이후 Action 내부에서 세부적인 요소에 따라 데이터를 갱신하는 방식이 있음
			// (다만 이 방식은 일일이 많은 양을 각기 쳐야 함)
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}

}
#endif