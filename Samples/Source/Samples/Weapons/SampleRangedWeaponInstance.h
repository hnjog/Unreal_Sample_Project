#pragma once

#include "CoreMinimal.h"
#include "SampleWeaponInstance.h"
#include "SampleRangedWeaponInstance.generated.h"

UCLASS()
class SAMPLES_API USampleRangedWeaponInstance : public USampleWeaponInstance
{
	GENERATED_BODY()
public:

	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	// 총탄 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;
};
