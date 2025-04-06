#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "SamplePlayerController.generated.h"

UCLASS()
class SAMPLES_API ASamplePlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ASamplePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
