#include "SampleGameState.h"
#include "SampleExperienceManagerComponent.h"

ASampleGameState::ASampleGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<USampleExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
