#include "CommonLocalPlayer.h"

FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;

	if (PlayerState)
	{
		// player stste가 설정되어 있다면 바로 호출
		Delegate.Execute(this, PlayerState);
	}

	// OnPlayerStateSet 등록
	return OnPlayerStateSet.Add(Delegate);
}
