#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SampleGameState.generated.h"

class USampleExperienceManagerComponent;

UCLASS()
class SAMPLES_API ASampleGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ASampleGameState();

public:
	UPROPERTY()
	TObjectPtr<USampleExperienceManagerComponent> ExperienceManagerComponent;

};
