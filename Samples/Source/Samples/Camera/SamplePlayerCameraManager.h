#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SamplePlayerCameraManager.generated.h"

// Controller에 바인딩된 CameraManager

#define SAMPLE_CAMERA_DEFAULT_FOV (80.0f)
#define SAMPLE_CAMERA_DEFAULT_PIRCH_MIN (-89.0f)
#define SAMPLE_CAMERA_DEFAULT_PIRCH_MAX (89.0f)

UCLASS()
class SAMPLES_API ASamplePlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ASamplePlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
