#include "SamplePlayerController.h"
#include "Samples/Camera/SamplePlayerCameraManager.h"
#include "Samples/AbilitySystem/SampleAbilitySystemComponent.h"
#include "SamplePlayerState.h"

ASamplePlayerController::ASamplePlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ASamplePlayerCameraManager::StaticClass();
}

void ASamplePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	// UPlayerInput :: ProcessInputStack에 의해 호출
	if (USampleAbilitySystemComponent* SampleASC = GetSampleAbilitySystemComponent())
	{
		SampleASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ASamplePlayerState* ASamplePlayerController::GetSamplePlayerState() const
{
	// ECastCheckedType::NullAllowed 는 Null 반환을 의도하는 경우 사용
	return CastChecked<ASamplePlayerState>(PlayerState,ECastCheckedType::NullAllowed);
}

USampleAbilitySystemComponent* ASamplePlayerController::GetSampleAbilitySystemComponent() const
{
	const ASamplePlayerState* SamplePS = GetSamplePlayerState();
	return (SamplePS ? SamplePS->GetSampleAbilitySystemComponent() : nullptr);
}
