#include "SampleTaggedActor.h"

// Sets default values
ASampleTaggedActor::ASampleTaggedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASampleTaggedActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASampleTaggedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

