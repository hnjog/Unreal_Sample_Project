#include "CommonPlayerController.h"
#include "CommonLocalPlayer.h"

void ACommonPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	// PlayerController가 LocalPlayer에 붙으면 활성화되는 이벤트가 ReceivedPlayer()
	if (UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);
	}

}
