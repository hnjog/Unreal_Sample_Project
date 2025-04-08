#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SampleAnimInstance.generated.h"

UCLASS()
class SAMPLES_API USampleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 해당 속성은 Lyra의 AnimBP에서 사용되는 값이므로 정의 필요
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
