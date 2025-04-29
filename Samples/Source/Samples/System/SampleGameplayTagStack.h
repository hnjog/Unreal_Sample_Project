#pragma once

#include "CoreMinimal.h"
#include <GameplayTagContainer.h>
#include "SampleGameplayTagStack.generated.h"

// 무언가를 대표하는 gameplay tag + 그 개수 count
USTRUCT(BlueprintType)
struct FSampleGameplayTagStack
{
	GENERATED_BODY()
public:
	FSampleGameplayTagStack() {}
	FSampleGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		:Tag(InTag),
		StackCount(InStackCount)
	{}

public:
	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 StackCount = 0;
};

/*
	lyra는 Item Base의 시스템 구조

	위 구조체의 Container
	이걸 다양한 스탯 관리에 사용 가능
*/

USTRUCT(BlueprintType)
struct FSampleGameplayTagStackContainer
{
	GENERATED_BODY()
public:
	FSampleGameplayTagStackContainer() {}

	void AddStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}
public:
	UPROPERTY()
	TArray<FSampleGameplayTagStack> Stacks;

	// Lookup Table (array 방식은 순회를 해야하기에)
	// 일부 요소에 대한 '개수' 만 필요한 경우에 대하여 더 빠르게 사용할 수 있도록 
	// 추가로 저장
	TMap<FGameplayTag, int32> TagToCountMap;
};