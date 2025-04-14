#pragma once

#include "CoreMinimal.h"
#include "Samples/Equipment/SampleGameplayAbility_FromEquipment.h"
#include "SampleGameplayAbility_RangedWeapon.generated.h"

class USampleRangedWeaponInstance;

// 어디 대상으로 Taget으로 설정할지 옵션들 (Lyra의 경우, 다양한 옵션 존재)
UENUM(BlueprintType)
enum class ESampleAbilityTargetingSource : uint8
{
	// Camera 기준으로 진행
	// 3인칭 카메라 시점이라 그런지
	// Pawn의 위치에서 쏘면 괴리감이 심함
	// 그렇다고 카메라 위치에서만 사용하게 하면, 무기 위치에서의 괴리감이 심해짐
	// 그렇기에 무기의 위치를 가져와,
	// Camera의 직선 위치에 맞게 가져옴(벡터의 투영? 내적?)
	CameraTowardsFocus,
};

UCLASS()
class SAMPLES_API USampleGameplayAbility_RangedWeapon : public USampleGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		USampleRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{
		}
	};

	USampleGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	void PerformLocalTargeting(TArray<FHitResult>& OutHits);

	FTransform GetTargetingTransform(APawn* SourcePawn, ESampleAbilityTargetingSource Source);
	FVector GetWeaponTargetingSourceLocation() const;
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	// called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	USampleRangedWeaponInstance* GetWeaponInstance();
};
