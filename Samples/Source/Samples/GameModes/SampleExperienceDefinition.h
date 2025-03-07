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

	// ��ŷ�� ����
	// ���� ��忡 ���� GameFeature plugin �ε�, �̿� ���� �����
	// ShooterCore�� ���õ� �۾� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
