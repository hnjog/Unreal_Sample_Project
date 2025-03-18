#include "SamplePlayerCameraManager.h"

ASamplePlayerCameraManager::ASamplePlayerCameraManager(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	// 가로 시야각 및 세로 시야각도 제한
	DefaultFOV = SAMPLE_CAMERA_DEFAULT_FOV;
	ViewPitchMin = SAMPLE_CAMERA_DEFAULT_PIRCH_MIN;
	ViewPitchMax = SAMPLE_CAMERA_DEFAULT_PIRCH_MAX;
}
