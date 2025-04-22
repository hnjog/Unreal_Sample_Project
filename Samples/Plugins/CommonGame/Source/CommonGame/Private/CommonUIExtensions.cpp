#include "CommonUIExtensions.h"
#include "CommonActivatableWidget.h"
#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"

UCommonActivatableWidget* UCommonUIExtensions::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	// LocalPlayer -> GameUISubsystem 가져옴
	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		// UIManager에서 현재 활성화된 UIPolicy를 가져옴
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			// policy에서 Localplayer에 맞는 PrimaryGameLayout을 가져옴
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				// PrimaryGameLayout, W_OverallUILayout의
				// LayerName에 Stack으로 WidgetClass(UCommonActivatableWidget)을 붙여줌
				return RootLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
			}
		}
	}

	return nullptr;
}
