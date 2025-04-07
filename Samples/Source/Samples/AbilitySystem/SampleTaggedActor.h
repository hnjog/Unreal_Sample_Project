#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SampleTaggedActor.generated.h"

UCLASS()
class SAMPLES_API ASampleTaggedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASampleTaggedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
