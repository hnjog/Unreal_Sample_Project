#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "SampleExperienceManagerComponent.generated.h"

UCLASS()
class SAMPLES_API USampleExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	

};
/*
	GameStateComponent (Plugin 기능)
	- UGameStateComponent -> 결국 UActorComponent를 상속받은 녀석
	-> 단순히 Actor에만 붙이는 '컴포넌트'를 GameState 등에 붙일 수 있도록 하는 클래스
	(GameState에 붙이는 추가적인 기능 같은 것)
	(-> Unity에서 다양한 GameObject에 컴포넌트를 붙일 수 있듯)

	GameFeature 시스템기능으로 unity처럼 부품을 붙였다 뗄수 있음
	(확장성이 좋아짐 , 디버깅이 힘들어지고, 부품이 많아질 순 있음)
*/
