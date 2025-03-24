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

	// TargetOffsetCurve�� �������̵� �Ǿ� �ִٸ� Curve�� ���� �����ͼ� ���� ����
	// Camera ���忡�� Character�� ��� �κп� Target���� ���� �����ϴ� ���
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
