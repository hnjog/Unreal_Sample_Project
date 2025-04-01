#include"GameFeatureAction_WorldActionBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_WorldActionBase)

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// 월드 순회
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		// 앞서 GameFeatureAction을 활성화 하면서 Context에 World를 넣어줬기에
		// 이에 따라 적용할 대상인지를 판단
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			// WorldActionBase의 interface 함수 호출하여
			// 해당 월드에 Action 적용
			AddToWorld(WorldContext, Context);
		}
	}
}
