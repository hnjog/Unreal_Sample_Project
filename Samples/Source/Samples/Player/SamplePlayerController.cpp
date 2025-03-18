#include "SamplePlayerController.h"
#include"../Camera/SamplePlayerCameraManager.h"

ASamplePlayerController::ASamplePlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ASamplePlayerCameraManager::StaticClass();
}
