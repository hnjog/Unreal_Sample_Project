﻿#include "SampleCameraComponent.h"

USampleCameraComponent::USampleCameraComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USampleCameraComponent::OnRegister()
{
	Super::OnRegister();

	// CameraComponent는 World의 Tick을 따라가는 것이 아니라
	// PlayerController에서 호출하는 UpdateCamera를 따라가게 된다
	// (직접적인 Tick을 따르는 것이 아닌 , Player 에 소속된다는 점이 특이하단 걸까)
	// 
	// PlayerController. UpdateCamera
	// -> PlayerCameraManager.CalcCamera () : 시야를 전반적으로 고려하여 출력(렌더링) (매 프레임 호출)
	// -> Character.GetCameraView() : 찍은 화면 의 정보를 얻어옴
	// -> CameraComponent : 화면을 찍는 용도
	// 이런식으로 PlayerController의 하위에 존재
	// 
	// PlayerCameraManager에서 렌더링을 하는 이유?
	// CameraComponent 자체는 Character에 종속됨
	// -> 다른 캐릭터에게도 빙의되는 등의 일이 가능하며,
	//    그러한 경우는 카메라 전환이 필요할 수 있음
	//	(카메라는 캐릭터를 따라 다녀야 하지만,
	//   화면 자체를 그리는 역할이 캐릭터에 종속이 되면 안되기에)
	// 
	// PlayerController -> PlayerCameraManager -> Charcter -> CameraComponent
	// 이러한 호출 구조를 가지는 듯
	// 
	if (!CameraModeStack)
	{
		// 초기화(BeginPlay)가 필요없는 객체이므로 NewObject로 할당
		// UObject를 Spawn시키는 경우는 NewObject, (ConstructObject는 이제 사용하지 않는다고 한다)
		// AActor 기반이라면 SpawnActor 를 고려 가능하다
		// (BeginPlay 등은 Actor의 생명주기 이벤트)
		// 
		// CreateDefaultSubobject  : AActor의 서브 오브젝트인 '컴포넌트','메쉬' 등을 생성할 때 사용
		// (서브 오브젝트 : 객체가 다른 객체의 구성 요소로 포함될때 사용하는 용어)
		// 
		CameraModeStack = NewObject<USampleCameraModeStack>(this);
	}

}

// 매 프레임 호출됨
void USampleCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	// EvaluateStack
	// - CameraModeStack에 있는 CameraMode를 업데이트(+블렌딩) 하고,
	//   CameraModeStack을 Bottom-Top 까지 업데이트된 CameraMode들을 lerp 진행
	// 이 결과가 CameraModeView에 캐싱
	FSampleCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			// PlayerController의 ControlRotation을 CameraModeView의 ControlRotation으로 업데이트
			// - PC가 Possess하고 있는 Pawn의 RootComponent에 ControlRoatation을 반영(조정하고 있는 캐릭터에 회전)
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera의 Location/Rotation을 반영
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	// FOV Update
	FieldOfView = CameraModeView.FieldOfView;

	/*
		ControlRotation : PlayerController가 조정하는 Pawn의 Rotation을 적용할 값
		Rotation : 카메라에 반영하는 Rotation
	*/

	// FMinimalViewInfo 를 업데이트하여, 변화 사항을 최종 렌더링까지 반영
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
	

}

void USampleCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate는 CameraMode class를 반환한다
	// - 해당 Delegate는 HeroComponent의 멤버 함수로 바인딩
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<USampleCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			// 이 함수를 통해 매 프레임마다 카메라 모드를 저장
			// 이후, Blending에 이용
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
