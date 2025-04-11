#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SamplePlayerState.generated.h"

class USampleExperienceDefinition;
class USamplePawnData;
class USampleAbilitySystemComponent;

UCLASS()
class SAMPLES_API ASamplePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASamplePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// AActor's interface
public:
	virtual void PostInitializeComponents() final;

	// member method
public:
	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	// 폰 데이터 캐싱을 위하여 Experience 로딩 완료때 가져오기 위함
	void OnExperienceLoaded(const USampleExperienceDefinition* CurrentExperience);
	void SetPawnData(const USamplePawnData* InPawnData);
	USampleAbilitySystemComponent* GetSampleAbilitySystemComponent() const { return AbilitySystemComponent; }

public:
	// Cacheing 하는 이유? (ExperienceManagerComponent에도 있지 않나?)
	// 나중에 PlayerState에 GAS 컴포넌트를 붙일 예정
	// -> GAS가 폰데이터를 참조할 것이기에 미리 캐싱 코드를 만들어 놓음
	UPROPERTY()
	TObjectPtr<const USamplePawnData> PawnData;

	UPROPERTY(VisibleAnywhere, Category = "Sample|PlayerState")
	TObjectPtr<USampleAbilitySystemComponent> AbilitySystemComponent;
};
