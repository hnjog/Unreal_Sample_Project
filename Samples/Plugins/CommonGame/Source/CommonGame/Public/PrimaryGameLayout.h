#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PrimaryGameLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract)
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

public:
	// Layer를 추가하며, GameplayTag를 할당
	// (함수 전달에는 low pointer를 여전히 사용 ->
	// 별도의 객체 소유권을 '생기지' 않게 하기 위하여)
	UFUNCTION(BlueprintCallable, Category = "Layer")
	void RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);

public:
	// GameplayTag 와 CommonActivatableWidgetContainerBase 매핑
	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};

/*
	가장 큰 레이아웃(하나만 존재)

	이 내부에 다양한 UI를 세팅하기에,
	제일 먼저 등록되어야 한다
*/