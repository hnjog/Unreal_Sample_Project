#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SamplePawnData.generated.h"

class USampleCameraMode;
class USampleInputConfig;

UCLASS()
class SAMPLES_API USamplePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	USamplePawnData(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());
	
	// Pawn의 class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sample|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Camera Mode
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sample|Camera")
	TSubclassOf<USampleCameraMode> DefaultCameraMode;

	// Input Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sample|InputConfig")
	TObjectPtr<USampleInputConfig> InputConfig;
};
