#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "SampleGameplayAbilityTargetData_SingleTarget.generated.h"

USTRUCT()
struct FSampleGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FSampleGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{

	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FSampleGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}


public:
	UPROPERTY()
	int32 CartridgeID;
};