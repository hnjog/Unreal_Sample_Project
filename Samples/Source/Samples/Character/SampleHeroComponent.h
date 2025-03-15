#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "SampleHeroComponent.generated.h"

/*
	카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트
	-> Character에서 처리하게 되는 경우가 보통임(SetUpPlayerInputComponent)
	=> 이 경우 캐릭터가 '입력'에 종속적이게 된다(의존성)

	UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
	-> 블루프린트에서 사용 가능 
	-> meta : 블루프린트 생성 가능

	일반적으로 생성하면 다 되던데??
	-> 기본적으로는 상속받는 클래스에 존재함
	(UPawnComponent의 상위 클래스인 UGameFrameworkComponent에
	Blueprintable 는 존재하나, Meta 부분이 없음)

	모든 키워드를 알 순 없지만, 
	일단 알아두면 구글링을 통해 되새김질 가능
*/

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class SAMPLES_API USampleHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	USampleHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
