#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"

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

UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
};
