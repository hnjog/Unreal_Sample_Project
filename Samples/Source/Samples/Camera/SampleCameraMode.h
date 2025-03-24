#pragma once

#include "CoreMinimal.h"
#include "SampleCameraMode.generated.h"

struct FSampleCameraModeView
{
	FSampleCameraModeView();

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/*
	Camera Blending 대상 유닛

	SAMPLES_API?
	 : 클래스나 함수가 DLL에서 외부로 노출될 때 사용
	 (다른 클래스나 함수가 다른 모듈에서 접근 가능하다는 것을 명시)
	 (이것을 제거하면 해당 모듈 내에서만 사용 가능)

	UCLASS(Abstract,NotBlueprintable)
	 : 추상 클래스 선언 -> 이 클래스의 인스턴스를 직접 생성 x (상속받아야 한다)
	 (NotBlueprintable -> 블루 프린트에서 사용 x)

	CameraMode?
	-> 카메라 시점의 '모듈화'를 위한 개념
	 (ex : 배그라면?
	 1인칭 조준 시점,
	 탑승물을 타는 운전 시점,
	 3인칭 일반 시점 등이 존재)

*/

UCLASS(Abstract,NotBlueprintable)
class USampleCameraMode : public UObject
{
	GENERATED_BODY()
public:
	USampleCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Member Method
	void UpdateCameraMode(float DeltaTime);
	void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

public:
	FSampleCameraModeView View;

	// Camera Mode의 FOV
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170"))
	float FieldOfView;

	// View에 대한 Pitch [Min, Max]
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	// Camera Blending 진행 시간
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// Blend 값 (0,1)
	float BlendAlpha;

	// CameraMode의 최종 Blend값
	// BLendAlpha의 선형 값을 매칭하며 BlendWeight를 계산
	float BlendWeight;

};

// Camera Blending 을 담당하는 객체
UCLASS()
class USampleCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	USampleCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USampleCameraMode* GetCameraModeInstance(TSubclassOf<USampleCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<USampleCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FSampleCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FSampleCameraModeView& OutCameraModeView) const;

	// Member Variables
public:
	// 생성된 CameraMode를 관리
	UPROPERTY()
	TArray<TObjectPtr<USampleCameraMode>> CameraModeInstances;

	// Camera Matrix Blend 업데이트 진행용 컨테이너
	// 이렇게 굳이 Stack으로 관리해야 하나?
	// 
	// 카메라를 '순간 이동' 시키지 않기 위하여
	// (게임의 몰입도를 위함)
	//
	UPROPERTY()
	TArray<TObjectPtr<USampleCameraMode>> CameraModeStack;
};