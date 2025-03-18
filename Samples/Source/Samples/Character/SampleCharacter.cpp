#include "SampleCharacter.h"
#include"SamplePawnExtensionComponent.h"
#include"../Camera/SampleCameraComponent.h"

ASampleCharacter::ASampleCharacter()
{
	// Tick 꺼준다
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	// PawnExtensionComponent 생성
	// native C++ 에서 선언한 것이 '먼저 호출'된다
	// BP에서 생성한 Simple Construction (BP에서 붙인 것들)이 이후에 호출된다
	// -> 엔진의 생성 순서를 이용하여
	// 컴포넌트 생성 순서를 관리할 수 있음
	// (Pawn은 여기서 생성되지만, BP에 생성되는 컴포넌트들은 이 생성자 이후에 생성이 되기에)
	// (다만 이후 생성되는 컴포넌트들은 BP에서만 생성해야 하는 점을 상기)
	// (더 파고드려면 Post Initialize 등을 참고하자)
	PawnExtComponent = CreateDefaultSubobject<USamplePawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent 생성
	// 이건 카메라에 다는 것이라 괜찮은 건가?
	// 어쩌면 UCameraComponent를 상속받은것이라 조금 다른 방식일듯?
	// (PawnExt,Hero : UGameFrameworkComponent 를 상속 - 플러그인 기능)
	// (UCameraComponent : SceneComponent를 상속)
	{
		CameraComponent = CreateDefaultSubobject<USampleCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
}

void ASampleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASampleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었기에
	// PawnExtensionComponent에게 현재 상황을 전달
	// 이러면 다시 PEC 와 HC에서 InitState가 업데이트 될것임
	PawnExtComponent->SetupPlayerInputComponent();
}

