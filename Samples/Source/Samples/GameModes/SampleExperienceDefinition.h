#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SampleExperienceDefinition.generated.h"

class USamplePawnData;

/*
	게임 정보를 담고 있을 클래스
	Experience : 게임의 모드와 매우 유사
	(단순 rule 등뿐 아니라 로딩화면, 메인 로비, 로그인 화면 등도 만들 수 있음)

	DefaultPawnData : pawn들을 spawn하기 위한 정보를 담음
	
	게임 모듈로 정의한 기능단위 구현을
	GameFeature로 더 세분화 가능

	ex) FirstPersonShooter로 정의한 GameFeature가
	Elemination Experience에 필요하다면 추가 하듯

*/

UCLASS()
class SAMPLES_API USampleExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	USampleExperienceDefinition(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<USamplePawnData> DefaultPawnData;

	// 마킹과 기억용
	// 게임 모드에 따른 GameFeature plugin 로딩, 이에 대한 연결고리
	// ShooterCore와 관련된 작업 이후 진행
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
