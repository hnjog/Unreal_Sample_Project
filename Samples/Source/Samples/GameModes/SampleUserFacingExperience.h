#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SampleUserFacingExperience.generated.h"

/*
	Map과 Experience의 정보를 가지고 있는 클래스
	Map - UserFacingExperience의 맵 정보
	Experience - 유저가 경험할 rule과 게임 정보 객체
*/

class UCommonSession_HostSessionRequest;

UCLASS()
class SAMPLES_API USampleUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Map 로딩 및 Experience 전환을 위해, MapID와 ExperienceID를 활용하여 HostSessionRequest를 생성
	// const 가 붙을 시, BlueprintPure가 자동으로 true가 되며
	// 실행단위로 표현되지 않기에 주의
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;

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
// UPrimaryDataAsset
// - GetPrimaryAssetId()에 대한 구현과 Asset Bundle에 대한 구현이 추가된 Data Asset
//  해당 기능은 AssetManager에서 load/unload 관련 메서드 호출로 활성화 가능
// 
// - GetPrimaryAssetId()를 통해 2가지 패턴 구현 가능
//  1. UPrimaryDataAsset를 상속받은 Native(c++) 클래스 정의하면, 
//     그 정의된 클래스를 PrimaryAssetType으로서 사용 가능
//	   (USampleExperienceDefinition)
//  2. UPrimaryDataAsset를 상속받은 BluePrint는 아래의 2가지 방식에 따라 PrimaryAssetType이 정의됨
//     - 부모(Parent Class)를 거슬러 올라가며 가장 처음으로 만난 Native(C++) 클래스
//     - 또는 가장 부모인 BP 클래스 (아마 해당 BP가 UPrimaryDataAsset를 상속받음)
//  - 이와 같은 동작 방식을 바꾸고 싶다면 GetPrimaryAssetId가 virtual이므로 override 하면 됨
//  (CDO로 가져다 사용하지 않으면 클래스 이름을 AssetID로 사용해버림, 그 경우 AssetName도 동일해짐)
//  (CDO가 아니면 '스캔'도 곤란해진다)
//  (AssetType이 클래스명이기에 BluePrint로 만든 버전을 사용하기 힘들어짐)
//  (CDO라면 예외처리가 되기에 괜찮아진다)
//
