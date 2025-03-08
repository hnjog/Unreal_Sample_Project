#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SampleUserFacingExperience.generated.h"

/*
	Map과 Experience의 정보를 가지고 있는 클래스
	Map - UserFacingExperience의 맵 정보
	Experience - 유저가 경험할 rule과 게임 정보 객체
*/

UCLASS()
class SAMPLES_API USampleUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// map에 대한 정보
	// meta 를 이용하여 관련 정보만 저장하기 위함
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	// 폰 데이터
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
// -> 게임 모드에서 이들에 관한 데이터를 한번에 읽을 수 있음
// Experience 는 기존의 unreal gamemode가 너무 무겁기에 나온 기능 (가볍게 만든 게임 모드?)
// 
//
