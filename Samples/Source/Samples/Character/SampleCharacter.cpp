#include "SampleCharacter.h"

ASampleCharacter::ASampleCharacter()
{
	// Tick 꺼준다
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
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

