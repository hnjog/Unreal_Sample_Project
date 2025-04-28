#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SampleAttributeSet.h"
#include "SampleCombatSet.generated.h"

/*
	전투와 관련된 Attribute를 담고 있는 Set 클래스
	이 클래스는 누가 들고 있어야 할까?
	-> AbilitySystemComponent와 함께 위치해야 함(무조건?)
*/

UCLASS()
class SAMPLES_API USampleCombatSet : public USampleAttributeSet
{
	GENERATED_BODY()
public:
	USampleCombatSet();

	ATTRIBUTE_ACCESSORS(USampleCombatSet, BaseHeal);

public:
	/*
		FGameplayAttribute가 참고하는 실제 AttributeSet에 있는 데이터
		(float 말고 사용하는 구조체)

		Heal의 단위로 쓰일 변수 (5.0f 이라면 period 당 5 만큼 Heal 된다)

		여기서는 heal 등의 여러 상황을 관리한 후
		실제 적용은 SampleHealthSet이 해줄 예정
		(ex:
		 heal 아이템 먹음 + 동료가 힐 해줌
		 -> 총 수치 15?
		 => 이러한 수치를 SampleHealthSet에 넘겨주어
		    실제 적용시킴)
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Sample|Combat")
	FGameplayAttributeData BaseHeal;
};
