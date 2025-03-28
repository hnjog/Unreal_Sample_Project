#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"

// UCommonSession_HostSessionRequest를 만들어
// CommonSessionSubsystem 전달하면 내부에서
// MapID와 CmdArgs로 최종적인 CmdArgs를 만들어줌

UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HostSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	// MapID -> Text로 변환
	FString GetMapName() const;

	// ServerTravel에 전달할 최종 URL 생성
	FString ConstructTravelURL() const;

public:
	// 준비할 MapID (맵 경로)
	UPROPERTY(BlueprintReadWrite, Category = Session)
	FPrimaryAssetId MapID;

	// 전달할 CmdArgs (Experience 이름 전달)
	UPROPERTY(BlueprintReadWrite, Category = Session)
	TMap<FString, FString> ExtraArgs;
}


/*
	Subsystem?
	(엔진, 에디터, GameInstance, World, LocalPlayer 등 5개 존재)
	-> 싱글톤과 비슷하게 사용(고유성 및 쉬운 접근, 생성 후 사라지지 않음)
	-> 자체 엔진이라면 실제 싱글톤을 구현해도 되나,
	   언리얼은 생명주기가 복잡
	-> 우리가 생성하여 PIE 시작시 생성하고, 종료하더라도
	   엔진 상에 남아있음
	   (다음 PIE 시작시 이전에 종료했던 데이터를 참고할 수 있음)
	
	특정 부분에서 전역적인 접근과 고유성을 보장하되,
	특정 부분과 생명주기를 같이하는 무언가가 필요해...!
	-> Subsystem (Unreal 4.2에 이러한 상황을 보완하기 위해 나왔다고 한다)
	
	ex
	- Engine Subsystem은 엔진의 생명주기를 따름
	- World Subsystem은 특정 월드에선 싱글톤처럼 접근 가능하나, 그 월드가 파괴되는 순간 사라진다
	  (다른 월드로 넘어간다던가 등 - 이 땐 다른 월드 서브시스템 생성)

*/

// UCommonSessionSubsystem 은 SampleGame에서 사용되어야 하기에
// Module Export를 해줘야 하고, 그렇기에 COMMONUSER_API를 사용
// {ModuleName}_API의 추가는 '다른 모듈'에서 사용할 경우 추가하면 됨
UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UCommonSessionSubsystem() {}

	UFUNCTION(BlueprintCallable, Category = Session)
	void HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request);

public:
	// PendingTravelURL은 맵의 경로
	FString PendingTravelURL;
};
