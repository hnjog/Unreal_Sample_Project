#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SamplePawnExtensionComponent.generated.h"

/*
	'확장'을 관리하는 컴포넌트
	초기화 전반을 담당하게 될 클래스

	컴포넌트 추가에 따라서, 초기화 순서를 관리
	(다른 컴포넌트가 이 컴포넌트에 종속적인 것이 될지?)
*/

UCLASS()
class SAMPLES_API USamplePawnExtensionComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	USamplePawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};

/*
	Gameplay actor plugin의 기능 중 일부

	PawnComponent
	-> Pawn에 부착할 수 있는 Component 타입이라는 뜻
*/
