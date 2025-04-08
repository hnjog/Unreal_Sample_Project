#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SampleInventoryItemDefinition.generated.h"

// Inventory에 대한 Fragment?
// -> Lyra에서는 특정한 Inventory 형식에 대하여 '정보'를 가지는 방식
// ex) InventoryFragment_EquippableItem : EquipmentItemDefinition을 가지고, 장착 가능한 아이템을 의미
//     InventoryFragment_SetStats : 아이템에 대한 정보를 가짐
//		- Rifle 형식이라면, '총알'에 대한 장착 최대치 및 현재 남은 총알 수 등등
// 
// 아이템 구현은 매우 다양한 방식이 존재
// 
// ex) ItemBase -> HealItem 이런식으로 상속 받는다던가
// (나중에 게임 크기가 커지게 되면 이 방식은 너무 많은 클래스가 만들어지긴 함)
// 
// (예전 개발하던 곳은 ItemBase 기반으로
// 장착,소비, 코스튬 등 '공통' 적인 부분(아이템 index 및 고유 id)은 냅두고
// 세부적인것은
// ItemValue1 이런식으로 다룬 후,
// 각자 '캐스팅'하여 사용)
// (클래스 자체는 총 3~4개 였던가)
// (다만 세부적인 아이템 옵션을 적용하는 부분이 좀 길었었다)
// (enum이 1000개가 넘었음)
// 
// 다만 결국 새로운 형식의 아이템이 나오고
// 그것이 기존의 분리한 것들의 특성이 합쳐지면 고민이 된다
// -> '횟수 제한 바주카'는 소모형 아이템과 장비형 아이템의 특징을 모두 갖는다
//    (이렇게 되면 어느쪽 클래스를 변경하거나, 새로운 클래스를 파야하는지에 대한 고민거리가 생김)
// 
// 언리얼 lyra에서 구현한 방식은
// Fragment(조각, 컴포넌트) 방식을 사용 (아이템 컴포넌트 같은 개념)
// Fragment를 이용하여
// '특성'을 부여하고 장착시키는 방식
// 
// ex) 권총 - '장착' Fragment 를 붙임 (장착 가능)
//     물약 - '소비' Fragment 붙임 (개수 소비 후 사라짐)
//     횟수 제한 바주카 - '장착','소비' Fragment를 각각 붙임
//
UCLASS(Abstract,DefaultToInstanced,EditInlineNew)
class USampleInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
};

UCLASS()
class SAMPLES_API USampleInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
	
};
