#pragma once

#include "CoreMinimal.h"
#include "Samples/Cosmetic/SampleCosmeticAnimationTypes.h"
#include "Samples/Equipment/SampleEquipmentInstance.h"
#include "SampleWeaponInstance.generated.h"

UCLASS()
class SAMPLES_API USampleWeaponInstance : public USampleEquipmentInstance
{
	GENERATED_BODY()
public:
	USampleWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Weapon에 적용할 AnimLayer를 선택하여 반환
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;
public:
	// Weapon에 Equip / UnEquip에 대한 Animation Set 정보
	// 각각 장착과 해제의 애니메이션을 관리 (모든 무기라도 장착과 해제가 있으므로)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FSampleAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FSampleAnimLayerSelectionSet UnequippedAnimSet;
};
