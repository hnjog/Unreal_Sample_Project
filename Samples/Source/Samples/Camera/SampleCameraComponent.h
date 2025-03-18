#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SampleCameraComponent.generated.h"

UCLASS()
class SAMPLES_API USampleCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	USampleCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
