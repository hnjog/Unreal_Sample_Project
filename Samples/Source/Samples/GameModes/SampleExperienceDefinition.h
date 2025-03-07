#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SampleExperienceDefinition.generated.h"

class USamplePawnData;

UCLASS()
class SAMPLES_API USampleExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	USampleExperienceDefinition(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<USamplePawnData> DefaultPawnData;

	// 마킹과 기억용
	// 게임 모드에 따른 GameFeature plugin 로딩, 이에 대한 연결고리
	// ShooterCore와 관련된 작업 이후 진행
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
