#pragma once

#include "CoreMinimal.h"
#include <GameplayTagContainer.h>
#include "SampleCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FSampleAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

public:
	// AnimLayer를 적용할 대상 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	// Cosmetic Tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FSampleAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

public:
	// AnimLayer 적용할 SkeletalMesh를 들고 있음 -> Animation-Mesh간 Rules를 MeshRules라 생각하자
	// Animation과 Mesh 를 연동하는 용도
	// (인간형 액터에 괴물 메시와 연동되게 하면 기괴하므로)
	// 인간형 액터에는 '인간'이라는 Tag를 붙이므로서 판단 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSampleAnimBodyStyleSelectionEntry> MeshRules;

	// Default로 적용할 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// Physics Asset은 하나로 동일 -> 모든 Animation의 Physics 속성은 공유
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};