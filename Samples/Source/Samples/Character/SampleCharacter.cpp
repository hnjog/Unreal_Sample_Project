#include "SampleCharacter.h"

ASampleCharacter::ASampleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

