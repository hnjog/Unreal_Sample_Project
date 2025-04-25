#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SampleWeaponUserInterface.generated.h"

class USampleWeaponInstance;

UCLASS()
class SAMPLES_API USampleWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	USampleWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Weapon변경에 따른 bp 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(USampleWeaponInstance* OldWeapon, USampleWeaponInstance* NewWeapon);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	// 현재 장착된 WeaponInstance 추적 (NativeTick 활용하여 업데이트)
	// 사실 Delegate가 더 효율적
	UPROPERTY(Transient)
	TObjectPtr<USampleWeaponInstance> CurrentInstance;
};
