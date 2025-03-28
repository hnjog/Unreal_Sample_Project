#include "SampleUserFacingExperience.h"
#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* USampleUserFacingExperience::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	
	// UCommonSession_HostSessionRequest는 UObject로 생성하면, 알아서 GC가 됨
	// 해당 객체는 현재 프레임에서 사용하기에 GC에 대한 염려	
	// - 만약 다음 프레임이나 추가적인 프레임에서 사용하면 LifeTime 관리가 필요 (안그러면 dangling)
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
