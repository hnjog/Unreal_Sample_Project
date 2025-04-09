#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SampleEquipmentDefinition.generated.h"

class USampleEquipmentInstance;

USTRUCT()
struct FSampleEquipmentActorToSpawn
{
	GENERATED_BODY()
public:
	// Spawn할 대상 Actor 클래스 (사실상 Actor를 상속받은 에셋)
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	// 어느 Bone Socket에 붙일지 결정
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttackSocket;

	// Socket에서 어느정도 Transformation을 더할 것인지 (Rotation, Position, Scale)
	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

// USampleEquipmentDefinition : 장착 아이템에 대한 정의 클래스 (메타 데이터)
UCLASS(Blueprintable)
class SAMPLES_API USampleEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	USampleEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// 해당 메타 데이터를 사용하면 어떤 Instance를 Spawn할지 결정하는 클래스
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<USampleEquipmentInstance> InstanceType;

	// 해당 장착 아이템을 사용하면, 어떤 Actor가 Spawn이 되는지 정보를 담음
	// ex) rpg의 장비들(모자,장갑,신발 등...)
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FSampleEquipmentActorToSpawn> ActorsToSpawn;
};
