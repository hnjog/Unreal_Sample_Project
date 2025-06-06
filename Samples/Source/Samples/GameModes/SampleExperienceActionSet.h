﻿#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SampleExperienceActionSet.generated.h"

class UGameFeatureAction;

// GameFeatureAction을 담는 DataAsset
// GameFeature가 Action을 기준으로 기능
// 그런데 다양한 게임모드에 따라 Action이 달라질 수 있음
// 그러한 Action들을 모아놓는 일종의 Wrapping 클래스

UCLASS()
class SAMPLES_API USampleExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	USampleExperienceActionSet();
#if WITH_EDITORONLY_DATA
	// 이 함수는 에셋 변경 or 리빌드 등이 필요할때, 에디터가 내부에서 호출
	virtual void UpdateAssetBundleData() override;
#endif
public:
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
