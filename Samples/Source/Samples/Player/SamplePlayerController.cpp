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
	// (입력 이벤트 발생 시 ProcessInputStack 가 호출되고,
	// 그 후, 입력 처리 결과를 반영하기 위해 마지막으로 PostProcessInput 가 호출됨)
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
