#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SampleUserFacingExperience.generated.h"

UCLASS()
class SAMPLES_API USampleUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// map�� ���� ����
	// meta �� �̿��Ͽ� ���� ������ �����ϱ� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	// �� ������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "SampleExperienceDefinition"))
	FPrimaryAssetId ExperienceID;

};

// UserFacing
// - Map
// - Experience
//   - PawnData
//      - Pawn
//      - Input
//		- Ability
// 
// -> ���� ��忡�� �̵鿡 ���� �����͸� �ѹ��� ���� �� ����
// Experience �� ������ unreal gamemode�� �ʹ� ���̱⿡ ���� ��� (������ ���� ���� ���?)
// 
//
