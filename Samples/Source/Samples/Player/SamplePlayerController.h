#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SamplePlayerController.generated.h"

UCLASS()
class SAMPLES_API ASamplePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASamplePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
