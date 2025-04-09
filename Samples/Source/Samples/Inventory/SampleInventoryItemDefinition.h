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


// Abstract - 추상 클래스로 정의(직접 인스턴스화 불가)
// DefaultToInstanced - 이 '클래스'를 사용하는 객체 생성시, 이 객체도 인스턴스화 됨 (보통 Editor나 BP에서 생성될때 고려 가능)
// EditInlineNew - 이 클래스의 인스턴스가 Editor의 디테일 패널에서 수정 가능하도록 함 (쉬운 수정)
UCLASS(Abstract,DefaultToInstanced,EditInlineNew)
class USampleInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
};

// 기본적으로는 '메타데이터'를 들고 있는것이 Definition의 개념
UCLASS(Blueprintable)
class SAMPLES_API USampleInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	USampleInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// Inventory Item 정의(메타) 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	// Inventory Item의 Component를 Fragment로 인식하면 됨
	// 아이템의 특성을 여기 붙인다
	// EditDefaultsOnly - 에디터에서 기본값만 편집
	// Instanced - 이 프로퍼티가 포함된 클래스가 인스턴스화 되어서 사용될 수 있음을 나타냄
	// (이 프로퍼티는 다른 객체에 대한 참조를 가지고, 그 객체는 독립적으로 존재 가능)
	// BlueprintReadOnly - 읽기 전용 (BP에서 수정 x)
	// Category = Display - Editor의 디테일 패널에서 특정한 카테고리에 속함을 보여준다
	//  - Category = "" 와 일반적으로 사용하는 차이는 딲히 없음 (가독성 여부)
	// 
	// Fragments 를 사용하기 위하여 3개의 키워드가 필요
	// Instanced - TObjectPtr 을 사용한 TArray 이고, 각각 별도의 객체이므로 사용
	// USampleInventoryItemFragment 쪽에서
	// DefaultToInstanced - 이 객체를 사용할 때, Fragment가 인스턴스화 됨 (BP에서 끼워주면 객체화)
	// EditInlineNew - Editor에서 그러한 것을 확인하고 편집을 쉽게 하기 위함
	// 
	// (Fragment 자체는 BP가 있는 Editor 상에서 가리키게 됨)
	// (사실상 자유로운 속성 조합을 위한 것)
	// 
	// 일반적으론 이러한 BP를 만들고 안에 Fragment를 집어넣더라도,
	// Editor의 Instance 상에서만 적용되고, CDO에는 반영이 안되어서 저장이 되지 않음
	// (즉, Editor의 Instance엔 적용이 되었지만 런타임의 생성된 객체에선 저장이 안됨)
	// -> 그렇기에 위의 키워드를 이용하여 실제 Instance가 생성될때도 적용되도록 저장
	// 
	UPROPERTY(EditDefaultsOnly,Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<USampleInventoryItemFragment>> Fragments;
};
