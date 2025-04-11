#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "SampleAbilitySet.generated.h"

class USampleGameplayAbility;
class USampleAbilitySystemComponent;

// GameplayAbility의 Wrapper용 구조체
// (추가적인 커스터마이징을 위함)
// InputTag를 이용하여 Ability를 쉽게 가져오기 위한 용도도 있음
USTRUCT(BlueprintType)
struct FSampleAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	// 허용된 GameplayAbility
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USampleGameplayAbility> Ability = nullptr;

	// Input 처리를 위한 GameplayTag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	// Ability의 허용 조건(Level)
	// 스킬 등에서 응용 가능
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FSampleAbilitySet_GrantedHandles
{
	GENERATED_BODY()
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(USampleAbilitySystemComponent* SampleASC);

protected:
	// 허용된 GameplayAbilitySpecHandle(int32) 값으로 비교 가능
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

// Ability를 담아놓는 DataAsset
// 다양한 모드 등에 따른 FSampleAbilitySet_GameplayAbility 를 들고 있다
UCLASS()
class SAMPLES_API USampleAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	USampleAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ASC에 허용 가능한 Ability 추가
	void GiveToAbilitySystem(USampleAbilitySystemComponent* SampleASC, FSampleAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

public:
	// 허용된 GameplayAbilities
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FSampleAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
