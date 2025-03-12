#include "SampleGameplayTags.h"
#include "SampleLogChannels.h"
#include"GameplayTagsManager.h"

FSampleGameplayTags FSampleGameplayTags::GameplayTags;

void FSampleGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FSampleGameplayTags::AddTag(FGameplayTag& outTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	// editor에서 등록
	outTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FSampleGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	// 태그 InitState 를 만들고 하위 태그를 넣어줌
	// Editor에서는 태그로 사용하고
	// C++에선 해당 변수들로 사용
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");
}
