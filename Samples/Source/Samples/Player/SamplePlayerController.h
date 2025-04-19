#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "SamplePlayerController.generated.h"

class ASamplePlayerState;
class USampleAbilitySystemComponent;

UCLASS()
class SAMPLES_API ASamplePlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	ASamplePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// PlayerController Interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	// Member Method
	ASamplePlayerState* GetSamplePlayerState() const;
	USampleAbilitySystemComponent* GetSampleAbilitySystemComponent() const;
};
