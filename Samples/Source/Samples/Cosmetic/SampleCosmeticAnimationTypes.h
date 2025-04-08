#pragma once

#include "CoreMinimal.h"
#include <GameplayTagContainer.h>
#include "SampleCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FSampleAnimLayerSelectionEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FSampleAnimLayerSelectionSet
{
	GENERATED_BODY()

public:
	// FSampleAnimBodyStyleSelectionSet의 MeshRule 처럼
	// AnimInstance의 Rule을 가진 LayerRules
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSampleAnimLayerSelectionEntry> LayerRules;

	// Default Layer
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultLayer;
};

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
	// GameplayTag를 통해 (CosmeticTags), Mesh Rules에 따라 알맞은 BodyStyle를 번환
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

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

/*
	각 상태에 맞는(총, 칼 무기 등) 애니메이션 Layer를 장착하기 위하여
	Tag 와 이러한 모듈 방식을 사용
	-> 당장은 이렇게 나누는 걸 까다롭게 느낄 수 있으나
	   Unreal 5의 방식이라 생각해보고 다시 공부해보자
*/