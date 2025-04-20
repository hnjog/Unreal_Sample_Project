#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "SampleLocalPlayer.generated.h"

// commonLocalplayer 래핑용 클래스 (원래 lyra에선 기능이 있지만)
// 일단 클론코딩이라 따라만 생성
UCLASS()
class SAMPLES_API USampleLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()
	
};
