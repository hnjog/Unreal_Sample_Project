#include "SamplePlayerState.h"
#include"../GameModes/SampleExperienceDefinition.h"
#include"../Character/SamplePawnData.h"
#include"../GameModes/SampleExperienceManagerComponent.h"

void ASamplePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	USampleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<USampleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ASamplePlayerState::OnExperienceLoaded(const USampleExperienceDefinition* CurrentExperience)
{
}
