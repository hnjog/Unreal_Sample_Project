#include "SampleCosmeticAnimationTypes.h"

USkeletalMesh* FSampleAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	// MeshRule을 순회하며, CosmeticTags 요구 조건에 맞는 MeshRule을 찾아 SkeletalMesh를 반환
	for (const FSampleAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if ((Rule.Mesh) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}
	return DefaultMesh;
}
