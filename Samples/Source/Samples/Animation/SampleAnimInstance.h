#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "SampleAnimInstance.generated.h"

// 다만 Lyra와의 클래스 이름이 다르기에 마이그레이션 시키기 까다로움
// 1. 이 클래스의 이름을 lyra와 동일하게
// 2. lyra 에셋을 이 클래스로 일일이 변환
// -> 둘다 무척 까다로움
// 
// 마이그레이션 시 자동으로 교체 시켜줌 (Script : Source를 뜻함)
// DefaultEngine.ini
// [CoreRedirects]
// +ClassRedirects = (OldName = "LyraAnimInstance", NewName = "/Script/Samples.SampleAnimInstance")
//
UCLASS()
class SAMPLES_API USampleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// Animation이 최초로 초기화 될때 호출
	virtual void NativeInitializeAnimation() override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

public:
	// 해당 속성은 Lyra의 AnimBP에서 사용되는 값이므로 정의 필요
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;

	// GameplayTag와 AnimInstance의 속성값을 매핑
	// 
	// FGameplayTagBlueprintPropertyMap
	// - AnimInstacne의 boolean 값과
	//   GameplayTag를 서로 매칭시켜준다
	//   (Tag 부여로 해당 매핑된 boolean 값을 true,
	//    제거로 false로 만들 수 있음)
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
