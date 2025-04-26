#include "CircumferenceMarkerWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CircumferenceMarkerWidget)

UCircumferenceMarkerWidget::UCircumferenceMarkerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCircumferenceMarkerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyMarkerWidget.Reset();
}

void UCircumferenceMarkerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MyMarkerWidget->SetRadius(Radius);
	MyMarkerWidget->SetMarkerList(MarkerList);
}

// Widget의 경우 다 TSharedRef 를 사용
TSharedRef<SWidget> UCircumferenceMarkerWidget::RebuildWidget()
{
	MyMarkerWidget = SNew(SCircumferenceMarkerWidget)
		.MarkerBrush(&MarkerImage)
		.Radius(this->Radius)
		.MarkerList(this->MarkerList);
	return MyMarkerWidget.ToSharedRef();
}