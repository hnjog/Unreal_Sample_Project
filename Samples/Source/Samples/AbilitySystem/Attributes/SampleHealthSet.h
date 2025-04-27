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
	// Attribute_Accessors를 통해 설정
	
	ATTRIBUTE_ACCESSORS(USampleHealthSet, Health);
	ATTRIBUTE_ACCESSORS(USampleHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(USampleHealthSet, Healing);

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
