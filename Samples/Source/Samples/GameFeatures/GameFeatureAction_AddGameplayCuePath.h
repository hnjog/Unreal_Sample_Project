#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCuePath.generated.h"

// WorldBase 상속은 안받는다
// (WorldBase - 각 World마다 구분해야 할때)
// 
// 이 기능은 Gameplay Cue에서 사용하며,
// 모든 World에서 필요함(World1 에서 등록하였는데, 다른 World에서 갑자기 못쓰는것은 그것대로 이상할지도?)
UCLASS()
class UGameFeatureAction_AddGameplayCuePath : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	UGameFeatureAction_AddGameplayCuePath();

	// GameplayCueNotify 에셋 경로 추가
	UPROPERTY(EditAnywhere, Category = "GameFeature|GameplayCues")
	TArray<FDirectoryPath> DirectoryPathsToAdd;
};