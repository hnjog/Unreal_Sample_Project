#pragma once

#include "SampleMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FSampleMappableConfigPair
{
	GENERATED_BODY()
public:
	FSampleMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};