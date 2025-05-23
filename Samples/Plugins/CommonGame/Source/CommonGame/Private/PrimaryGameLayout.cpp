﻿#include "PrimaryGameLayout.h"

UPrimaryGameLayout::UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

UCommonActivatableWidgetContainerBase* UPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerName)
{
	return Layers.FindRef(LayerName);
}

void UPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	// bp에서 호출하여 넣어줄 용도
	if (!IsDesignTime())
	{
		// commonUI 에 존재하는 '전환 속도' 설정
		LayerWidget->SetTransitionDuration(0.0);
		Layers.Add(LayerTag, LayerWidget);
	}
}
