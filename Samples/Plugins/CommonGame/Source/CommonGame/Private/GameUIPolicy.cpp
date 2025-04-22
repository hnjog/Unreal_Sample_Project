#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "CommonLocalPlayer.h"

UPrimaryGameLayout* UGameUIPolicy::GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo? LayoutInfo->RootLayout : nullptr;
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer)
{
	// 로딩해서 넘겨줌
	return LayoutClass.LoadSynchronous();
}

// LocalPlayer에 UPrimaryGameLayout Widget 생성 및 세팅
void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer)
{
	// PlayerController가 할당되었을 경우,
	// LayoutWidget을 생성
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		// LayoutWidgetClass가 있을 경우 
		// (PrimaryGameLayout은 Abstract이기에 상속받는 class인지 체크)
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			// PlayerController가 소유한다는 의미에서 Owner를 설정
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);

			// push_back과 비슷하지만,
			// 배열 내에서 바로 생성자 호출(복사/이동 비용 x)
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutFromViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

void UGameUIPolicy::AddLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// CommonUserWidget에 PlayerContext 지정
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));

	// 해당 Layout에 우선순위를 높게 설정(1000)
	// - AddToPlayerScreen을 통해 Widget을 붙임
	// (우선순위를 높게 설정하여, PrimaryLayout을 가장 뒤쪽에 배치되도록한다)
	Layout->AddToPlayerScreen(1000);
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// UCommonUserWidget의 SlateWidget을 가져와
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		// Parent와 연결고리 끊어놓음
		// 해당 layout이 localplayer에서 떼어짐
		Layout->RemoveFromParent();
	}
}

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	// PlayerController가 업데이트되면 Gamelayout을 업데이트 해주기 위해
	// Delegate를 추가 (컨트롤러가 Localplayer에 설정되었을 때, UI 설정해야 하기에)
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this]
	(UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController) 
		{
			// 우선 추가된 Player가 있으면 제거
			// 아래쪽에서 이미 추가된 경우에 대하여
			// 근데 이러면 그냥 여기만 해도 되지 않나?
			NotifyPlayerRemoved(LocalPlayer);

			// RootViewportLayouts를 순회하며 검색
			if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
			{
				// Layout만 업데이트
				AddLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
				LayoutInfo->bAddedToViewport = true;
			}
			else
			{
				// layout을 생성하고 활성화
				CreateLayoutWidget(LocalPlayer);
			}
		});

	// 당장 될 수도 있지만, 현재 시점에서 설정이 안될 수 있기에
	// Delegate에 걸어놓는다
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		// Layout만 업데이트
		AddLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		// layout을 생성하고 활성화
		CreateLayoutWidget(LocalPlayer);
	}
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	// FindByKey (map 말고도 array에서 사용 가능)
	// (사실 for 순회랑 같은 방식)
	// 구조체에서 operator == 정의하였기에 가능
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer,LayoutInfo->RootLayout);

		// 비활성화 확인할 용도
		LayoutInfo->bAddedToViewport = false;
	}
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);

	// Player가 Distroy 되므로 Delegate에서 제거
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);

	// RootViewportLayouts에서 제거
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		// 만약 PrimaryGameLayout 존재시, Viewport에서도 제거
		UPrimaryGameLayout* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);
		RemoveLayoutFromViewport(LocalPlayer, Layout);
	}
}
