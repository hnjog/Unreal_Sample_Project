#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CommonGameInstance.generated.h"

// LocalPlayer가 PlayerController에 등록되는 것을 확인하고
// PrimaryGameLayout을 등록

UCLASS(Abstract)
class COMMONGAME_API UCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// UGameInstance Interface
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;

public:
	// 메인 로컬 플레이어 캐싱
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;

};
