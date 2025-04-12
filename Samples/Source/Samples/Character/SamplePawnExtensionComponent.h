#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
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

	IGameFrameworkInitStateInterface 는
	초기화를 편리하게 만들 수 있는 인터페이스
*/

class USamplePawnData;
class USampleAbilitySystemComponent;

UCLASS()
class SAMPLES_API USamplePawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	USamplePawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static USamplePawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<USamplePawnExtensionComponent>() : nullptr); }

	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const USamplePawnData* InPawnData);
	void SetupPlayerInputComponent();

	// AbilitySystemComponent의 AvatorActor 대상 초기화 / 해제 호출
	void InitalizeAbilitySystem(USampleAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitalizeAbilitySystem();

	USampleAbilitySystemComponent* GetSampleAbilitySystemComponent() const { return AbilitySystemComponent; };

public:
	// UPawnComponent Interfaces
	// OnRegister 
	// : 생성되는 초반에 호출됨
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// GetFeatureName -> 기본적으로는 Override해서 사용해야 한다
	// NAME_None을 반환하고 있기에
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

public:
	// Pawn을 생성한 Data를 캐싱 (다른 컴포넌트에서 사용 가능)
	UPROPERTY(EditInstanceOnly, Category = "Sample|Pawn")
	TObjectPtr<const USamplePawnData> PawnData;

	// AbilitySystemComponent 캐싱
	UPROPERTY()
	TObjectPtr<USampleAbilitySystemComponent> AbilitySystemComponent;

	// 선언은 헤더, 구현은 cpp
	static const FName NAME_ActorFeatureName;
};

/*
	Gameplay actor plugin의 기능 중 일부

	PawnComponent
	-> Pawn에 부착할 수 있는 Component 타입이라는 뜻
*/
