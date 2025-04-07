#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SampleCharacterPartTypes.h"
#include "SampleCosmeticAnimationTypes.h"
#include "SamplePawnComponent_CharacterParts.generated.h"

class USamplePawnComponent_CharacterParts;

// 인스턴스화된 Character Part의 단위
USTRUCT()
struct FSampleAppliedCharacterPartEntry
{
	GENERATED_BODY()
public:
	// Character Part의 정의(메타 데이터)
	UPROPERTY()
	FSampleCharacterPart Part;

	// FSampleCharacterPartList 에서 할당 받은 Part 핸들 값
	// (FSampleControllerCharacterPartEntry의 Handle 값과 같아야 함 -> 같으면 같은 Part라 판단)
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// 인스턴스화 된 Character Part용 Actor (스폰용 정보)
	// FSampleCharacterPart 에서 Actor의 Class 데이터를 들고 있으므로,
	// 그것을 실제 생성하기 위함 (-> 또는 메시정보를 바꾼다던가 할때 제거해야할 수도 있음)
	//
	// UChildActorComponent?
	// - Actor에 자식으로 Actor를 붙일때 사용하는 컴포넌트
	// (Actor와 Actor의 결합에 사용한다)
	//
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

// USamplePawnComponent_CharacterParts 에서 실질적 Character Parts를 관리하는 클래스
// 실질적인 로직을 들 구조체
USTRUCT(BlueprintType)
struct FSampleCharacterPartList
{
	GENERATED_BODY()
public:
	FSampleCharacterPartList()
		:OwnerComponent(nullptr)
	{
	}

	FSampleCharacterPartList(USamplePawnComponent_CharacterParts* InOwnerComponent)
		:OwnerComponent(InOwnerComponent)
	{
	}

public:
	//현재 인스턴스화 된 Character Part
	UPROPERTY()
	TArray<FSampleAppliedCharacterPartEntry> Entries;

	// 해당 FSampleCharacterPartList의 Owner인 PawnComponent
	UPROPERTY()
	TObjectPtr<USamplePawnComponent_CharacterParts> OwnerComponent;

	// PartHandle의 값을 할당 및 관리하는 변수
	int32 PartHandleCounter = 0;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class USamplePawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	USamplePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// 인스턴스화 된 Character Parts
	UPROPERTY()
	FSampleCharacterPartList CharacterPartList;

	// 애니메이션 적용을 위한 메시와 연결고리
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FSampleAnimBodyStyleSelectionSet BodyMeshes;
};

/*
	전반적인 개념은
	ControllerComponent에선 가벼운 Data 위주로 들고 있으며,
	실제 무거운 것은 이쪽의 PawnComponent 에서 들고 있는다.

	ControllerComponent 에서 지시를 내릴때,
	CharacterPartList 의 적용할 녀석들을 적용

	이러한 분리는 '확정성'을 고려한 것이기도 하지만,
	Lyra 프로젝트는 Dedicate라는 Unreal 네트워크 기반으로 구성
	(네트워크의 입출력은 그리 빠르지 않음)
	(Network Serilized Array 시스템을 사용하는 이유
	- 특정 요소가 변경되었는지를 감지하고 (리플렉션 시스템)
	  그 요소만을 업데이트)
	(이 시스템을 이용하려면,
	 Array 역할을 담당하는 부분과 그 Array 요소 가 분리되어 있어야 가능)
	 (List 와 Entry)
	 이 시스템을 사용하기 위하여 위와 같은 분리가 이루어짐
*/