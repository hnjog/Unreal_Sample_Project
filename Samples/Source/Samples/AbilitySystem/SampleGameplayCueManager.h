#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "SampleGameplayCueManager.generated.h"

/*
	Gameplay Cue는 Effect를 
	보다 쉽게, '반응형 트리거'로서 이용하게 만드는
	일종의 렌더링 관련 정보

	그말은?
	-> 서버에선 굳이 메모리에 로드할 필요 없음
*/
UCLASS()
class SAMPLES_API USampleGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	USampleGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	static USampleGameplayCueManager* Get();

	void RefreshGameplayCuePrimaryAsset();

};
