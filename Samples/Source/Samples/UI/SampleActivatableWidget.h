#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SampleActivatableWidget.generated.h"

// Input 처리 방식 
UENUM(BlueprintType)
enum class ESampleWidgetInputMode :uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

// UCommonActivatableWidget의 특성?
// - layout과 instance를 구분하기 위한 용도로
//   UCommonActivatableWidget은 layout의 정의
//   (런타임 활성화 / 비활성화 하기 쉬움 -> layout 껐다가 다른 layout 키는)
//   (WidgetTree에서 제거가 아닌 껏다 켰다)
// 
// ESampleWidgetInputMode : Input에 대한 처리 방법
// 
UCLASS()
class SAMPLES_API USampleActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	USampleActivatableWidget(const FObjectInitializer& ObjectInitializer);

	// TOptional?
	// C++의 std::optional과 비슷
	// (값의 존재 여부를 표현 , 값이 있을 수도, 없을 수도 있는 템플릿 클래스)
	// (pointer의 null 체크와 비슷)
	// 내부 처리 실패시, 값을 반환하지 않는다는 의미
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

public:
	// Input 처리 방식
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ESampleWidgetInputMode InputConfig = ESampleWidgetInputMode::Default;

	// Mouse 처리 방식
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCatureMode = EMouseCaptureMode::CapturePermanently;
};
