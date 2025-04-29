#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "SampleReticleWidgetBase.generated.h"

class USampleWeaponInstance;
class USampleInventoryItemInstance;

UCLASS()
class USampleReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	USampleReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(USampleWeaponInstance* InWeapon);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();
public:
	// WeaponInstance / InventoryIntance 를 상태 추적용 캐싱
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USampleWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USampleInventoryItemInstance> InventoryInstance;
};
