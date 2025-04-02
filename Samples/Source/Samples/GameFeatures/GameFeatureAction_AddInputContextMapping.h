#pragma once

#include "GameFeatureAction_WorldActionBase.h"
#include "UObject/SoftObjectPtr.h"
#include "GameFeatureAction_AddInputContextMapping.generated.h"

class UInputMappingContext;

USTRUCT()
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AssetBundles = "Client,Server"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 Priority = 0;

	/** If true, then this mapping context will be registered with the settings when this game feature action is registered. */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bRegisterWithSettings = true;
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Input Mapping"))
class UGameFeatureAction_AddInputContextMapping final : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

public:
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
};