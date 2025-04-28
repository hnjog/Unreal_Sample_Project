#pragma once

#include "CoreMinimal.h"
#include "SampleAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SampleHealthSet.generated.h"

/*
	FGameplayAttributeData 
	- float 기반 래핑 클래스
	(그냥 Float 쓰지 말라 주석이 있긴 하다)

	BaseValue : 기본값
	CurrentValue : 현재수치값
	-> 기본값을 알아야 현재 수치와 얼마만큼 '다른지' 알 수 있음
*/

UCLASS()
class SAMPLES_API USampleHealthSet : public USampleAttributeSet
{
	GENERATED_BODY()
public:
	USampleHealthSet();

	// Attribute_Accessors를 통해 설정
	ATTRIBUTE_ACCESSORS(USampleHealthSet, Health);
	ATTRIBUTE_ACCESSORS(USampleHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(USampleHealthSet, Healing);

	// Attribute 값을 ClampAttribute를 활용하여, 값의 범위를 유지
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	// 특정 Attribute 값이 바뀌기 전에 거치는 함수들을
	// override 함으로서 일정한 범위 내를 유지하도록 한다(위의 ClampAttribute 호출)
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// GameplayEffect가 HealthSet의 Attribute를 수정하기 전에 불리는 Callback 함수
	// - Healing이 Update되면 이걸 기반으로 Health에 Healing을 적용하여 update 가능
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
public:
	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "Sample|Health")
	FGameplayAttributeData Health;

	// 최대 체력
	UPROPERTY(BlueprintReadOnly, Category = "Sample|Health")
	FGameplayAttributeData MaxHealth;

	// 회복 수치
	UPROPERTY(BlueprintReadOnly, Category = "Sample|Health")
	FGameplayAttributeData Healing;
};
