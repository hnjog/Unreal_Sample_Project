#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SampleCameraComponent.generated.h"

class USampleCameraModeStack;

UCLASS()
class SAMPLES_API USampleCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	USampleCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

public:
	UPROPERTY()
	TObjectPtr<USampleCameraModeStack> CameraModeStack;
};
