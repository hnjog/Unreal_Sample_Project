#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "SampleHealthComponent.generated.h"

class USampleAbilitySystemComponent;
class USampleHealthSet;
class USampleHealthComponent;
class AActor;
struct FOnAttributeChangeData;

// Character(Pawn)에 대해 체력관련 처리 담당 Component
// - Blueprintable (UI에서 바인딩하기 위하여)
// 
// UI <-> Status가 서로를 직접 참조하지 않도록
// 중간다리 역할을 맡는 클래스
// 
// UI가 Status를 직접 참여하게 되면, Status를 수정할 때,
//  UI 클래스를 수정해야 하는 일이 발생할 수 있음
// -> 서로의 종속성을 떨어트리는 역할을 하게 된다
//   + 변화에 따른 Event, Delegate 방식이 가능하기에
//    Tick 등을 사용하여 일일이 변화를 감지할 필요 없음
//    (음 이건 Status 쪽에 Delegate를 넣지 않기 위함일까?
//     Delegate는 이 클래스가 없더라도 사용이 가능하지 않을까 싶지만)
//    (어쩌면 Status class 내에 UI용 Delegate가 있는 것조차
//	   종속성을 늘려지는 것일수도)
// => UI를 써야하는 경우, 이런식으로 중간다리 역할을 하는 클래스를 계속 만드나? 싶다

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSampleHealth_AttributeChanged, USampleHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS()
class SAMPLES_API USampleHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
public:
	USampleHealthComponent(const FObjectInitializer& ObjectInitializer);

	// Actor 의 HealthComponent 반환
	UFUNCTION(BlueprintPure, Category = "Sample|Health")
	static USampleHealthComponent* FindHealthComponent(const AActor* Actor);

	// HealthSet의 Attribute에 접근하기 위한 BP Accessor 함수들
	UFUNCTION(BlueprintPure, Category = "Sample|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Sample|Health")
	float GetMaxHealth() const;

	// 0~1.0f
	UFUNCTION(BlueprintPure, Category = "Sample|Health")
	float GetHealthNormalized() const;
	
	// ASC와 HealthSet 초기화
	void InitializeWithAbilitySystem(USampleAbilitySystemComponent* InASC);
	void UninitializeWithAbilitySystem();

	// ASC를 통해, HealthAttribute 변경이 있을때의 Callback 함수
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

public:
	// HealthSet을 접근하기 위한 AbilitySystemComponent
	UPROPERTY()
	TObjectPtr<USampleAbilitySystemComponent> AbilitySystemComponent;

	// 캐싱된 HealthSet 레퍼런스
	UPROPERTY()
	TObjectPtr<const USampleHealthSet> HealthSet;

	// Health 변화에 따른 Delegate(Multicast)
	UPROPERTY(BlueprintAssignable)
	FSampleHealth_AttributeChanged OnHealthChanged;
};
