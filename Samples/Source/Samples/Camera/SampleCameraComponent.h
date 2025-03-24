#pragma once

#include "CoreMinimal.h"
#include"SampleCameraMode.h"
#include "Camera/CameraComponent.h"
#include "SampleCameraComponent.generated.h"

// template 전방 선언
// 템플릿 클래스를 '전방 선언'하는 것으로
// TSubclassOf 라는 템플릿 클래스는 차후,
// 템플릿 매개변수인 CASTCLASS_AActor 라는 클래스 타입을 다룰 것
// (참고로 TSubclassOf 는 이미 정의된 템플릿 클래스 -> 엔진 내부에 존재)
// 
// CASTCLASS_AActor : 해당 템플릿 클래스가 사용할 타입을 지정하는 매개변수로
// 해당 클래스와 그 서브 클래스들을 지정할 수 있다
// 
template<class CASTCLASS_AActor> class TSubclassOf;

// ReturnType, Delegate Name
// 
// USampleCameraMode가 완전히 정의되어 있어야 델리게이트를 문제없이 사용 가능
// 만약 class USampleCameraMode; 로 전방선언 하고 Delegate를 사용하는 경우,
// 해당 클래스의 멤버나 메서드에 접근할 수 없는 상황이 되어버리므로 주의할 것
// 
// RetVal : 반환타입이 맞아야 함 (여기서는 별도의 매개변수가 정의되지 않았기에 0개의 매개변수를 가지는 함수를 요구)
// (별도로 Dynamic이 붙어있지 않기에 정적 Delegate)
// 
// 정적 Delegate
// - 정적 함수여야 함 (컴파일 시기에 바운딩 됨)
// C++ 코드에서만 사용
// 
// Dynamic Delegate?
// - 동적 델리게이트 (런타임 함수 바인딩 지원) -> 메타 데이터를 포함하여 BluePrint에서 쉽게 사용 가능
// + UObject 타입을 파라미터로 받을 수 있음
//
DECLARE_DELEGATE_RetVal(TSubclassOf<USampleCameraMode>, FSampleCameraModeDelegate);

UCLASS()
class SAMPLES_API USampleCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	USampleCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static USampleCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<USampleCameraComponent>() : nullptr); }

	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	void UpdateCameraModes();
	AActor* GetTargetActor() const;

public:
	// Camera의 Blending 기능을 지원하는 Stack
	UPROPERTY()
	TObjectPtr<USampleCameraModeStack> CameraModeStack;

	// 현재 CameraMode를 가져오는 Delegate
	FSampleCameraModeDelegate DetermineCameraModeDelegate;
};
