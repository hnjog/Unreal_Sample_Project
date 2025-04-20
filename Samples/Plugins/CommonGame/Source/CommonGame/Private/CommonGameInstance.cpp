#include "CommonGameInstance.h"
#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
    // GameInstance에 관리하는 Player 컨테이너에 추가
    // (ReturnVal 은 새로 추가된 Index)
    int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
    
    if (ReturnVal != INDEX_NONE)
    {
        // PrimaryPlayer는 처음만 캐싱 (아마도 Index == 0인 맨 처음 player를 지정하는듯)
        if (!PrimaryPlayer.IsValid())
        {
            PrimaryPlayer = NewPlayer;
        }

        // GameUIManagerSubsystem 을 통해
        // NotifyPlayerAdded 호출로 GameLayout을 추가
        GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
    }

    return ReturnVal;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
    if (PrimaryPlayer == ExistingPlayer)
        PrimaryPlayer.Reset();

    GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(ExistingPlayer));
    return Super::RemoveLocalPlayer(ExistingPlayer);
}
