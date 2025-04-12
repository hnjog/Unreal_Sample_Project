#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "SampleCharacter.generated.h"

class USamplePawnExtensionComponent;
class USampleCameraComponent;

UCLASS()
class SAMPLES_API ASampleCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASampleCharacter();

	// IAbilitySystemInterface
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Actor
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Pawn에 Possess가 호출된 이후 호출되는 함수
	// Possess : playerController가 Pawn이나 Character를 제어하도록 설정할 때 호출하며,
	//		플레이어가 해당 캐릭터를 자신의 '컨트롤러'에 소유하게 된다
	// 
	// SetupPlayerInputComponent : 플레이어가 입력을 설정하는 데 사용
	//								(인풋 바인딩 : 이동 등)
	// 
	// 플레이어의 입력은 플레이어 컨트롤러로 가게 된다
	// 따라서 그러한 입력을 설정하기 위하여 Pawn에서 이 함수를 호출
	// 
	// 반대로 SetupPlayerInputComponent 가 호출이 되었다는 것은
	// Possess가 호출되었다는 증거
	// 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// TObjectPtr<> : 언리얼 내부의 스마트 포인터 같은 것
	// - UObject 기반 클래스에 대한 포인터
	// - 가비지 컬렉션 시스템과 함께 사용되어 성능 최적화
	// - 안전한 포인터 관리(dangling pointer 같은 상황을 방지)
	// -> Unreal 내부에서 UObject 기반한 메모리에 대하여 추적하고 관리하여
	//    가비지 컬렉터가 참조되지 않는 메모리(or 특정 조건 삭제)를 수거
	// => 개발자의 실수를 방지하며 개발 편의를 위한 기능
	// 
	// 에디터가 아닌 환경이라면
	// Raw Pointer로 변환되어 최적화 된다
	// 
	// 전반적으로 '게임 오브젝트'인 UObject 를 위한 '스마트 포인터' 이다
	// 언리얼 엔진에 이미 다른 스마트 포인터인
	// TSharedPtr - 여러 객체가 동일한 리소스를 공유(참조 카운트로 생명 주기 관리)
	// TWeakPtr - TSharedPtr 와 함께 사용되어 '약한 참조' 유지
	// TUniquePtr - 고유한 소유권 제공 (다른 포인터로 복사 x)
	// 등이 존재
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sample|Character")
	TObjectPtr<USamplePawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sample|Character")
	TObjectPtr<USampleCameraComponent> CameraComponent;
};
