#pragma once

#include "CoreMinimal.h"
#include "SampleCameraMode.h"
#include "SampleCameraMode_ThirdPerson.generated.h"

// 추상 클래스이기에 다른 클래스에서 상속받아야 객체 생성이 가능
UCLASS(Abstract,Blueprintable)
class SAMPLES_API USampleCameraMode_ThirdPerson : public USampleCameraMode
{
	GENERATED_BODY()
public:
	USampleCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
