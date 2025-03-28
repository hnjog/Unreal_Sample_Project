#include "CommonSessionSubsystem.h"
#include <Engine/AssetManager.h>

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	// AssetManager를 통해 MapID를, FAssetData를 반환
	// PackageName을 반환하여 Map 경로를 얻을 수 있음
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}

	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	return FString();
}
