#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SampleEquipmentInstance.generated.h"

struct FSampleEquipmentActorToSpawn;

// 일반적인 Definition(정의) <-> Instance (저장형) 과는 다르게
// 일부 데이터를 관리해준다
UCLASS(BlueprintType,Blueprintable)
class SAMPLES_API USampleEquipmentInstance : public UObject
{
	GENERATED_BODY()
	
public:
	USampleEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// BP 정의를 위한 Equip/Unequip 함수
	// BlueprintImplementableEvent : BP에서 정의할 함수이므로 여기선 구현 안한다
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

public:
	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;

	void SpawnEquipmentActors(const TArray<FSampleEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();

	// DeterminesOutputType은 C++ 정의에는 Apawn* 반환
	// BP에선 PawnType에 따라 OutputType이 결정되도록 Redirect
	// DeterminesOutputType : 함수의 출력 타입을 동적으로 설정
	// (PawnType 을 통해 TSubclassOf<APawn> PawnType 에서 변수로 들어온
	//  서브클래스 타입에 맞도록 출력 타입이 설정됨)
	UFUNCTION(BlueprintPure, Category = Equipment, meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	// interface
	virtual void OnEquipped();
	virtual void OnUnequipped();

public:
	// 어떤 InventroyItemInstance에 의해 활성화되었는지
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	// SampleEquipmentDefinition에 맞게 Spawn된 Actor Instance 들
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
