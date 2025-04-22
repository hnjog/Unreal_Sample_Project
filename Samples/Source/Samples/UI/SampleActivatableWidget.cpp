#include "SampleActivatableWidget.h"

USampleActivatableWidget::USampleActivatableWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

// Widget 활성화 시에 호출
TOptional<FUIInputConfig> USampleActivatableWidget::GetDesiredInputConfig() const
{
	// Menu에 들어갔을 때는,
	// 공격용 키가 눌리지 않는다던가의 상황을 만들 수 있음(Game 쪽에 input을 보내지 않는다던가)
	switch (InputConfig)
	{
	case ESampleWidgetInputMode::Default:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCatureMode);
	case ESampleWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCatureMode);
	case ESampleWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Menu, GameMouseCatureMode);
	case ESampleWidgetInputMode::Menu:
	default:
		return TOptional<FUIInputConfig>();
	}

}
