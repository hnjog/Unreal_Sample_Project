#include "SampleCameraMode.h"
#include"SamplePlayerCameraManager.h"
#include"SampleCameraComponent.h"

// 특정 클래스나 구조체에 대한 자동 생성된 C++ 파일을 포함하는 역할
// 지정된 클래스의 메타데이터를 통하여 리플렉션을 이용하기 위함
// -> SampleCameraMode : 클래스 이름 인자
// 
#include UE_INLINE_GENERATED_CPP_BY_NAME(SampleCameraMode)

FSampleCameraModeView::FSampleCameraModeView()
	:Location(ForceInit),
	Rotation(ForceInit),
	ControlRotation(ForceInit),
	FieldOfView(SAMPLE_CAMERA_DEFAULT_FOV)
{

}

void FSampleCameraModeView::Blend(const FSampleCameraModeView& Other, float OtherWeight)
{
	// 사실상 최신 데이터이기에, 이거랑 블렌딩할 필요 없음
	if (OtherWeight <= 0.0f)
	{
		return;
	}

	if (OtherWeight >= 1.0f)
	{
		// Weight가 1.0 이면 현재 개체에 Other을 덮어쓰면 된다
		// 1.0 이상인 경우는 이미 과거의 데이터이고,
		// 순회 순서상 Other이 더 과거의 것이기에,
		// 현재 Other는 보관할 필요가 없음
		// 그러므로 덮어씌워준다
		*this = Other;
		return;
	}

	//Location + OtherWeight * (Other.Location - Location)
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation += (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation += (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);

}

USampleCameraMode::USampleCameraMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	FieldOfView = SAMPLE_CAMERA_DEFAULT_FOV;
	ViewPitchMin = SAMPLE_CAMERA_DEFAULT_PIRCH_MIN;
	ViewPitchMax = SAMPLE_CAMERA_DEFAULT_PIRCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = ESampleCameraBlendFunction::EaseOut;
	BlendExpontent = 4.0f;
}

void USampleCameraMode::UpdateCameraMode(float DeltaTime)
{
	// Acotr를 활용하고, Pivot[Location | Rotation]을 계산해, View를 업데이트
	UpdateView(DeltaTime);

	// BlendWeight를 DeltaTime을 활용하여, BlendAlpha 계산 후,
	// BLendFunction에 맞게 재 매핑하여 최종 계산
	UpdateBlending(DeltaTime);
}

// Character의 Location과 ControlRotation을 활용하여, View를 업데이트
void USampleCameraMode::UpdateView(float DeltaTime)
{
	// 기존에는 SprintArm 컴포넌트를 이용한 회전 방식이나
	// Pivot의 개념은 spring arm 처럼 캐릭터의 위치에 따라 이동하게 하고, 회전값을 조정하는 역할
	
	// CameraMode를 가지고 있는 CameraComponent의 Owner인 Character(Pawn)을 활용해, PivotLoaction/Rotation을 반환
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch 값에 대해 Min, Max값을 Clamp시킨다
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FSampleCameraModeView에 PivotLocation/Rotation을 설정
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 ControlRotation으로 활용
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

}

void USampleCameraMode::UpdateBlending(float DeltaTime)
{
	// BlendAlpha를 DeltaTime을 통해 계산
	if (BlendTime > 0.f)
	{
		// BlendTime은 Blending 과정 총 시간 (초)
		// - BlendAlpha 는 0 -> 1로 (BlendAlpha : 누적합)
		// - DeltaTime을 활용해, BlendTime을 1로 볼 경우, 진행 시간을 누적
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	// BlendWeight를 [0,1]를 BlendFunction에 맞게 재매핑
	const float Exponent = (BlendExpontent > 0.0f) ? BlendExpontent : 1.0f;
	switch (BlendFunction)
	{
	case ESampleCameraBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ESampleCameraBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ESampleCameraBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ESampleCameraBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending : Invalid BlendFunction [%d]\n"),(uint8)BlendFunction);
		break;
	}
}

USampleCameraComponent* USampleCameraMode::GetSampleCameraComponent() const
{
	// USampleCameraMode를 생성하는 곳은 USampleCameraModeStack
	// - 해당 코드 내부에서 GetOuter()를 SampleCameraMode로 SampleCameraComponent로 설정
	// - USampleCameraModeStack::GetCameraModeInstance() (NewObject 할때,Outer를 Stack의 Outer, 즉 CameraComponent를 넣어줌)
	return CastChecked<USampleCameraComponent>(GetOuter());
}

AActor* USampleCameraMode::GetTargetActor() const
{
	const USampleCameraComponent* SampleCameraComponent = GetSampleCameraComponent();
	return SampleCameraComponent->GetTargetActor();
}

FVector USampleCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight(약 88)를 고려하여, ViewLocation을 반환
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator USampleCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRotation : 보통의 경우, Pawn의 ControlRotation을 반환
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
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
	// UpdateStack 진행 후, Stack 내부에는 BlendWeight가 1.0이하인 CameraMode만 남게 됨
	UpdateStack(DeltaTime);

	// Bottom->Top 까지 CameraModeStack에 대해 Blending 진행
	BlendStack(OutCameraModeView);
}

void USampleCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
		return;

	// CameraModeStack 순회하며 업데이트
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	// 최신 데이터부터 가장 마지막 데이터까지
	for (int32 StackIndex = 0; StackIndex < StackSize; StackIndex++)
	{
		USampleCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		// CameraMode의 BlendWeight가 1.0에 도달하면 해당 CameraMode을 제거
		// 1.0을 넘어가면 사용하지 않을 데이터
		// - 그렇기에 이 이후의 데이터들은 더 볼 필요도 없이 제거
		// -> 이러한 이유로 stack이라 이름 붙였지만 array에서 0부터 집어넣는 이유
		// 반대로 최신 데이터를 뒤쪽에 두는 진짜 stack 방식으로 하는 경우,
		// 앞쪽부터 데이터를 지워나가며, 당겨야 하기에 0부터 집어넣는 듯하다
		if (CameraMode->BlendWeight >= 1.0f)
		{
			// 최소한 하나의 카메라 모드는 남겨두기 위하여 (인덱스 0은 남겨두기 위해)
			// 비록 지금 1이 넘었더라도 그 다음것부터 삭제
			RemoveIndex = StackIndex + 1;
			RemoveCount = StackSize - RemoveIndex;
			break;
		}
	}

	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void USampleCameraModeStack::BlendStack(FSampleCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// Bottom -> Top 순서로 Blend 진행
	const USampleCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	// 참고로 가장 아래쪽의 BlendWeight은 1.0
	OutCameraModeView = CameraMode->View;

	// StackSize - 1은 위쪽에서 이미 View를 건네줬으므로,
	// 나머지 녀석들에 대한 Blending을 진행
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; StackIndex--)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// 현재의 데이터에 이미 쌓여진 데이터들을 Blending 해준다
		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}
