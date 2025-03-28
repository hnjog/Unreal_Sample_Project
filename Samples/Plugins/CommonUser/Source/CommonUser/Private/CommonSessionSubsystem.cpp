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
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if(ExtraArg.Key.IsEmpty())
		{
			continue;
		}

		// ?를 seperate로 복수개의 ExtraArg를 전달할 수 있도록 함
		// - Key값 유무에 따라, =(assignment)를 통해 CmdArg 생성
		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}

	return FString::Printf(TEXT("%s%s"),*GetMapName(),*CombinedExtraArgs);
}
