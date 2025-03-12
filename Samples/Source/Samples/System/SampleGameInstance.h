#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SampleGameInstance.generated.h"

UCLASS()
class SAMPLES_API USampleGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	// UGameInstance interface
	virtual void Init() override;
	virtual void Shutdown() override;

};
