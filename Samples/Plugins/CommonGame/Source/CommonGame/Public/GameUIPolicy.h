#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameUIPolicy.generated.h"

class UPrimaryGameLayout;
class UCommonLocalPlayer;

// LayOut과 LocalPlayer를 매칭시키는 용도의 구조체
USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	// LocalPlayer에 RootLayer가 설정되었는지의 여부
	UPROPERTY(Transient)
	bool bAddedToViewport = false;

public:
	bool operator==(const ULocalPlayer* OtherLocalPlayer) const
	{
		return LocalPlayer == OtherLocalPlayer;
	}

	FRootViewportLayoutInfo() {}

	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		:LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}

};

// 사실상 Manager에서 이 녀석을 들고 있으며,
// 실질적인 관리는 Policy 쪽에서 Player와 layout을 들며 관리
UCLASS(Abstract,Blueprintable)
class COMMONGAME_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()
public:
	// LocalPlayer에 바인딩된 UI의 Layout
	// 어떤 Layout 클래스를 사용할지 결정
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

	// 보통 싱글 게임에선 LocalPlayer - PrimaryGameLayout 하나 이지만
	// 멀티플레이라면 여러 개 가능
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};
