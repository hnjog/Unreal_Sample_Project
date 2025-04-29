#include "SampleGameplayTagStack.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SampleGameplayTagStack)

void FSampleGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
		return;

	if (StackCount > 0)
	{
		// 선형 순회
		// Tag가 많지 않으므로 이런 방식인지?
		// 많아진다면 Map으로 관리하는것이 더 효율적일듯 (10개 미만인 경우는 Array가 더 빠를수도)
		// 
		// lyra 에선 전반적인 데이터 개수가 적으면 Array로 만드는 경향이 있음
		// (애초에 Map의 접근은 o(1)이지만, 상황에 따라 다를수도 있고, 삽입
		// 삭제의 비용이 더 클 수 있으므로)
		for (FSampleGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
				return;
			}
		}

		// 새로운 것
		FSampleGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);

		TagToCountMap.Add(Tag, StackCount);
	}
}

void FSampleGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
		return;

	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; It++)
		{
			FSampleGameplayTagStack& Stack = *It;
			if (Stack.StackCount <= StackCount)
			{
				// 아래에서 return 하니까 그냥 바로 지워버리는 것
				// 원래는 return 없었으면 It 새로 지정해야 한다
				// Unreal에선 RemoveCurrent가 내부적으로 index를 하나 줄여준다
				It.RemoveCurrent(); 
				TagToCountMap.Remove(Tag);
			}
			else
			{
				const int32 NewCount = Stack.StackCount - StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
			}
			return;
		}
	}
}
