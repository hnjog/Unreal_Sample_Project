#pragma once

#include "CoreMinimal.h"
#include <GameplayTagContainer.h>
#include "Engine/DataAsset.h"
#include "SampleInputConfig.generated.h"

class UInputAction;

// GamePlayTag와 InputAction을 연결하는 래퍼 클래스
// 결과값(Action)과 Tag를 연결해주는 단위
USTRUCT(BlueprintType)
struct FSampleInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Category = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class SAMPLES_API USampleInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	USampleInputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	// 잘 안바뀌는 액션들 (ex : 이동, 마우스 움직임 등)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FSampleInputAction> NativeInputActions;

	// 그 외의 액션들
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FSampleInputAction> AbilityInputActions;
};
