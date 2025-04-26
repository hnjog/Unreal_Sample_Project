#include "GameFeatureAction_AddWidgets.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Samples/UI/SampleHUD.h"
#include "UIExtension.h"
#include "CommonUIExtensions.h"
#include "GameFeaturesSubsystemSettings.h"

void UGameFeatureAction_AddWidgets::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

}

void UGameFeatureAction_AddWidgets::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	for (const FSampleHUDElementEntry& Entry : Widgets)
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Entry.WidgetClass.ToSoftObjectPath().GetAssetPath());
	}
}

void UGameFeatureAction_AddWidgets::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// GameFrameworkComponentManager를 가져올
	// GameInstance와 World는 GameWorld여야 함
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			// Widget을 추가할 대상을 고정
			TSoftClassPtr<AActor> HUDActorClass = ASampleHUD::StaticClass();

			// WorldBase 와 마찬가지로 HandleActorExtension을 콜백으로 받음
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this,&ThisClass::HandleActorExtension,ChangeContext)
			);

			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddWidgets::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// Receiver인 ASampleHUD의 Removed / Added에 따라 Widget 을 추가 / 제거
	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) ||
		(EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) ||
		(EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
	{
		AddWidgets(Actor, ActiveData);
	}
}

void UGameFeatureAction_AddWidgets::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ASampleHUD* HUD = CastChecked<ASampleHUD>(Actor);

	// HUD를 통해 LocalPlayer를 가져오기
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		// Layout의 요청 순회
		for (const FSampleHUDLayoutRequest& Request : Layout)
		{
			// TSoftClassPtr나 TSoftObjectPtr 은 '경로'만을 가리키는 방식 (메모리에 '있'어야 가져옴)
			// TObjectPtr처럼 이미 생성된 객체 메모리 위치를 가리키는 방식은 아님
			// 그렇기에 Load 등으로 메모리에 올릴 필요가 있음
			// (이것은 물론 TObjectPtr 자체도 마찬가지로 메모리에 올려야 함
			//  -> 다만 BP나 Level에 배치된 Actor,Asset 등이라면 언리얼 자체 로딩 시스템이
			//  해당 객체를 메모리에 올려주기에 별도의 로딩을 할 필요는 없음)
			// 
			// + TSoftClassPtr을 사용하는 이유?
			// -> 'TObjectPtr'은 강한 참조를 가지기에, 가리키는 객체가 반드시 메모리에 로드 되어야 함
			// => 해당 객체는 메모리에 계속 상주하며, Unload 및 Lazy load 가 불가능
			// 
			// 그렇기에, 실제 객체를 메모리에 올리지 않되 '그 경로'만을 들고 있는
			// TSoftClassPtr을 통해 '필요할 때'만 메모리를 Load하여 좀 더 메모리를
			// 효율적으로 사용할 수 있음
			// 
			// + 그럼 TSoftClassPtr만 사용할 수 있지 않나?
			// - TObjectPtr은 메모리에 해당 객체를 계속 참조하기에,
			//  바로 역참조가 가능함 (성능상 Delat가 없음)
			// - 그러나 TSoft Ptr은 '메모리에 남아 있지 않을 수 있기에'
			//   '확인'하는 오버헤드가 존재
			// - player 나 중요한 객체의 경우는 '반드시 존재한다'는 가정이 있기에
			//   계속 메모리에 올려야 하며, 그렇기에 TObjectPtr로 지정하는 것이 효율적
			// 
			// Lyra에서
			// Client,Server 양 측 모두 반드시 필요한 데이터는
			// TObjectPtr로 설정
			// 
			// 한쪽에서만 필요한 데이터는 TSoftPtr로 설정
			// (텍스쳐, 사운드, 이펙트 등)
			// 
			// Widget은 '클라이언트'에서만 필요한 존재이며,
			// 서버쪽에선 필요 없음 (이런걸 메모리에 올릴 만큼 여유롭지 않음)
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Request.LayoutClass.Get())
			{
				ActiveData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,Request.LayerID,ConcreteWidgetClass));
			}
		}

		// Widget 순회하며, UIExtensionSubsystem에 추가
		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FSampleHUDElementEntry& Entry : Widgets)
		{
			ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		}
	}
}

void UGameFeatureAction_AddWidgets::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ASampleHUD* HUD = CastChecked<ASampleHUD>(Actor);

	// SampleHUD에 추가된 UCommonActivatableWidget 을 순회하며 비활성화
	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActiveData.LayoutsAdded)
	{
		if (AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		}
	}
	ActiveData.LayoutsAdded.Reset();

	// UIExtension 순회하며 Unregister
	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}
