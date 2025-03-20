#include "SampleCameraMode.h"
FSampleCameraModeView::FSampleCameraModeView()
	:Location(ForceInit),
	Rotation(ForceInit),
	ControlRotation(ForceInit),
	FieldOfView(SAMPLE_CAMERA_DEFAULT_FOV)
{

}

USampleCameraMode::USampleCameraMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}


USampleCameraModeStack::USampleCameraModeStack(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

USampleCameraMode* USampleCameraModeStack::GetCameraModeInstance(TSubclassOf<USampleCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances 에서 먼저 생성되어 있는지 확인
	// 기존 요소를 캐싱한 Array에서 먼저 체크
	for (USampleCameraMode* CameraMode : CameraModeInstances)
	{
		// CameraMode는 클래스 타입당 하나만 생기게 할 것
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// CameraModeClass에 알맞는 CameraMode의 인스턴스가 없다면 생성
	// GetOuter() : 외부 객체 반환 (Scene 등에서 보이는, 해당 객체의 소속)
	// CameraModeClass : USampleCameraMode의 서브클래스가 들어와도 정상적으로 생성하기 위함
	// NAME_None : 이름 지정하는 곳이지만, NAME_None 넣으면 이름 없는 객체가 생성
	// RF_NoFlags : 설정 플래그 (RF_NoFlags : 플래그 설정 x)
	//
	USampleCameraMode* NewCameraMode = NewObject<USampleCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	// CameraModeClass에 맞는 인스턴스를 가지고(관리하는) 컨테이너임
	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

// TSubclassOf : UClass를 내부적으로 참조하기에
// 클래스의 메타데이터에 접근이 가능
// 
// 해당 메서드의 요점은
// '새로운 카메라 모드'가 들어와도
// 어느정도는 기존의 '시간 경과 비율'을 유지하며
// 추가를 한다는 점으로 이해해 보자
void USampleCameraModeStack::PushCameraMode(TSubclassOf<USampleCameraMode>& CameraModeClass)
{
	// 유효한 클래스인지 확인 (nullptr)
	// &여도 nullptr 자체는 들어올 수 있음
	if (!CameraModeClass)
		return;

	// 매번 만들 수 없으니, 캐싱해서 이미 만들어놓은거 있는지 체크하고
	// 없으면 그때 생성하여 Array에 넣어준다 (최적화를 위한 캐싱)
	// '메모리'에 부담이 가는 경우?
	// -> 요새는 '메모리'가 부족하다고 말하긴 힘든 상황이긴 함
	//    + CameraMode는 캐릭터마다 많아봤자 10개 이상 가기 힘든 기획상의 이유가 존재
	//    (조준 모드, 일반 모드, 차,배,비행기 탑승 시야, 감시카메라 등등)
	//	  (시야를 다양한 각도로 보는 Class가 그렇게 많을지는 조금 의문이므로)
	USampleCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// CameraModeStack[0] 이미 가장 최근에 CameraMode가 Stack 되었으므로 종료
		// 이미 최신 카메라 모드라는 소리
		// 당장은 [0]에 넣는 이유를 알기 어렵지만 나중에 추가 메모 예정
		return;
	}

	// ExistingStackIndex는 CameraModeStack에서 CameraMode에 맞는 Index를 찾음
	int32 ExistingStackIndex = INDEX_NONE;

	// ExistingStackContribution은 위에서 아래로 최종 BlendWeight 값을 찾기 위해 초기값으로 1.0으로 설정
	// 0 : 최신 값, 1 : 가장 오래된 값
	// - 0~1 사이의 값으로 하되,
	//   새로운 모드가 들어와도 그 비율이 유지되어야 함
	// 
	float ExistingStackContribution = 1.0f;

	/**
	 * BlendWeight    |    ExistingStackContribution    |    ExistingStackContribution (accumulated)
	 * 0.1f           |    (1.0f) * 0.1f = 0.1f         |    (1.0f - 0.1f) = 0.9f
	 * 0.3f           |    (0.9f) * 0.3f = 0.27f        |    (1.0f - 0.3f) * 0.9f = 0.63f
	 * 0.6f           |    (0.63f) * 0.6f = 0.378f      |    (1.0f - 0.6f) * 0.63f = 0.252f
	 * 1.0f           |    (0.252f) * 1.0f = 0.252f     |
	 *                |    0.1f + 0.27f + 0.378f + 0.252f = 1.0f!
	 * 
	  주어진 1.0f 를 사용
	  1번째 BlendWeight 0.1f 를 만나고, 
	   1.0f * 0.1f = 0.1f

	  해당 모드가 0.1f 를 사용했으므로
	  남은것은 0.9
	  
	  0.9인 상태로 2번째 0.3f를 만남 
	  -> 2번째 모드가 남은 값의 '0.3까지'의 비율을 차지하고 있는 상황이다
	  
	  일단 남은 0.9의 시간에서
	  2번째에게 0.27을 할애함
	  
	  그리되면 
	   (1.0f - 0.3) : 2번째까지가 3할이므로,
	   * 0.9 : 실제 남은 시간
	   0.63f

	   3번째도 같다
	   3번째까지의 Weight 가중치는 0.6 이므로
	  남은 시간의 60프로인 0.378을 할당
	  이후 남은 시간은 0.252

	  더 자세한 내용은 PDF, CDF 참고
	  
	 */

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			// 찾는 카메라 인덱스
			ExistingStackIndex = StackIndex;
			// BlendWeight를 CameraMode에 추가해주자:
			// - 여기서 ExistingStackContribution 우리가 찾는 CameraMode를 찾았으니깐, 
			// 누적된 BlendWeight와 함께 BlendWeight을 곱하여, 루프를 빠져나온다
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			// 당연히 우리가 원하는 CamearMode가 아니니깐, 
			// InvBlendWeight = (1.0 - BlendWeight)를 곱해줘야, 값이 누적
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}
	// 해당 루프의 동작 원리는 앞서 적어놓은 표를 확인해보며 이해해보자.

	// 우리는 CameraMode를 Top으로 반영하기 위해, 당연히 중간에 있었다면, 제거하여 다시 Push 해줘야 한다
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// 중간에 없었다면 최신값이므로 0 세팅
		ExistingStackContribution = 0.0f;
	}

	// BlendTime이 0보다 크다는 것은 Blend을 얼마 시간동안 진행함을 의미 따라서, ExistingStackContribution을 적용
	// - 따라서 Blend하지 않는다면, BlendWeight를 1.0을 넣어 새로 넣는 CameraMode만 적용할 것이다
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	// 이러는건 비효율적이지만, 이유가 있나...?
	CameraModeStack.Insert(CameraMode, 0);

	// 마지막은 항상 1.0!
	CameraModeStack.Last()->BlendWeight = 1.0f;


}

void USampleCameraModeStack::EvaluateStack(float DeltaTime, FSampleCameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num]까지 순차적으로 Stack에 있는 CameraMode 업데이트
	UpdateStack(DeltaTime);

	// Bottom->Top 까지 CameraModeStack에 대해 Blending 진행
	BlendStack(OutCameraModeView);
}

void USampleCameraModeStack::UpdateStack(float DeltaTime)
{
}

void USampleCameraModeStack::BlendStack(FSampleCameraModeView& OutCameraModeView) const
{
}
