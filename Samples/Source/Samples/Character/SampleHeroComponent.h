#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SampleHeroComponent.generated.h"

/*
	카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트
	-> Character에서 처리하게 되는 경우가 보통임(SetUpPlayerInputComponent)
	=> 이 경우 캐릭터가 '입력'에 종속적이게 된다(의존성)
*/

UCLASS()
class SAMPLES_API USampleHeroComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	USampleHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
