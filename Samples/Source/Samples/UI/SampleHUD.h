#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SampleHUD.generated.h"

UCLASS()
class SAMPLES_API ASampleHUD : public AHUD
{
	GENERATED_BODY()
public:
	ASampleHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// GameFrameworkComponent의 AddReceiver를 위한 메서드들
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
