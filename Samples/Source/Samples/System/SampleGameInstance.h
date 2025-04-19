#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "SampleGameInstance.generated.h"

UCLASS()
class SAMPLES_API USampleGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
	
public:

	// UGameInstance interface
	virtual void Init() override;
	virtual void Shutdown() override;

};
