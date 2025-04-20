#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

class UGameUIPolicy;

UCLASS(Abstract, Config = Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Member Method
	void SwitchToPolicy(UGameUIPolicy* InPolicy);

public:
	// UGameInstanceSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

	// defalut UI Policy를 생성할 class
	UPROPERTY(Config,EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};
