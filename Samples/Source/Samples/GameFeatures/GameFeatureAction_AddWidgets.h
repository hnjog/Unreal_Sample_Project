#pragma once

#include "CoreMinimal.h"
#include "UIExtensionSystem.h"
#include <CommonActivatableWidget.h>
#include "GameplayTagContainer.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameFeatureAction_AddWidgets.generated.h"

// forward declarations
struct FComponentRequestHandle;

// HUD의 Layout 요청
USTRUCT()
struct FSampleHUDLayoutRequest
{
	GENERATED_BODY()
public:
	// UI의 레이아웃으로 CommonActivatableWidget을 사용
	// TSoftClassPtr-> Widget들은 무겁기에 클래스 메타 데이터만 들고 있는다
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	// PrimaryGameLayout의 LayerID
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag LayerID;
};

USTRUCT()
struct FSampleHUDElementEntry
{
	GENERATED_BODY()
public:

	// SampleHUDLayout 위에 올릴 대상이 되는 Widget Class
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UUserWidget> WidgetClass;

	// SampleHUDLayoutRequest에 올린 LayoutClass에 정의된 Slot(GameplayTag)
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};


// GFA_AddWidget은 형태를 정의하는 layout 과 
// 그 위에 올릴 Widget 객체들인 Widgets로 정의
UCLASS()
class SAMPLES_API UGameFeatureAction_AddWidgets : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;

		// Lyra에서 UIExtension으로 관리
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

public:
	// 형태를 표현
	// 특정한 GameplayTag에 widget을 장착하기 위함
	// (레이아웃이 TArray인 이유는 
	// 아마도 다양한 경우를 생각한 것 아닐까?)
	// -> 
	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FSampleHUDLayoutRequest> Layout;

	// '구성 요소'를 표현 
	// 내부에서 추가적인 Tag를 하나 더 사용하여
	// 그 위치에 Widget을 붙임
	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FSampleHUDElementEntry> Widgets;
};

/*
	게임 플레이 Tag를 2번 사용하여,
	'붙일 PrimaryGameLayout의 Stack'의 위치를 찾고,
	그 위치에 Widget을 붙인다
*/