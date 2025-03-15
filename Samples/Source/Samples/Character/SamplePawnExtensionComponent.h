#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SamplePawnExtensionComponent.generated.h"

/*
	'확장'을 관리하는 컴포넌트
	초기화 전반을 담당하게 될 클래스

	컴포넌트 추가에 따라서, 초기화 순서를 관리
	그러나, '종속적'이진 않게

	Pawn
	 - PawnExtension
	   - Hero
	   - ...

	폰의 다른 컴포넌트를 관리할 예정이나,
	얘가 먼저 생성이 되어야 다른 녀석들을 관리할 수 있음

	그런데 다른 컴포넌트에 대한 '종속성'을 '제거'하면서,
	이 녀석이 가장 먼저 생성이 되어야 함을 유지해야 하는 상황이 된다

	Tick을 끄기에,
	Delegate 처럼 이벤트 기반의
	필요에 따라 그때그때 효율적으로 처리하는 방식으로 구현

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
