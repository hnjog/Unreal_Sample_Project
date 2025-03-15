#include "SampleCharacter.h"
#include"SamplePawnExtensionComponent.h"

ASampleCharacter::ASampleCharacter()
{
	// Tick 꺼준다
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	// PawnExtensionComponent 생성
	PawnExtComponent = CreateDefaultSubobject<USamplePawnExtensionComponent>(TEXT("PawnExtensionComponent"));
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

}

