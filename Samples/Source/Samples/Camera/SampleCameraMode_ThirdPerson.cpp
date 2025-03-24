#include "SampleCameraMode_ThirdPerson.h"
#include"Curves/CurveVector.h"

USampleCameraMode_ThirdPerson::USampleCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void USampleCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::Clamp(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// TargetOffsetCurve가 오버라이드 되어 있다면 Curve에 값을 가져와서 적용 진행
	// Camera 입장에선 Character의 어느 부분에 Target으로 할지 결정하는 방식
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
