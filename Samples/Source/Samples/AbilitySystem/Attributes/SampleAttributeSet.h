#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "SampleAttributeSet.generated.h"

// Attribute Set에 Attribute 추가시, 선언 및 정의해야할 메서드에 대한 간략 버전 제공
// 
// ATTRIBUTE_ACCESSORS (SampleHealthSet,Health)는
// 아래의 메서드를 선언 및 정의해줌
// 
// static FGameplayAttribute GetSampleHealthSet() {...}
// float GetHealth() const {...}
// void SetHealth(float NewVal) {...}
// void InitHealth(float NewVal) {...}
//
// 세부적으로는 Get##PropertyName 같은 방식으로 정의됨
// (## : 양 옆의 요소를 합침)
// (직접 만들어도 상관없음 -> 자체적으로 매크로를 지원하고 있다는 건 알아두자)
//

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \

UCLASS()
class SAMPLES_API USampleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	USampleAttributeSet();
};
