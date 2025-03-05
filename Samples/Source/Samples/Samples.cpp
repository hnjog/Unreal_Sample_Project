// Copyright Epic Games, Inc. All Rights Reserved.

#include "Samples.h"
#include "Modules/ModuleManager.h"

class SamplesModule : public FDefaultGameModuleImpl
{
	// 모듈 : h + cpp 로 이루어진 최소한의 기능
	// 플러그인 : 다양한 실험적, 외부 기능 (모듈을 여러개 포함할 수 있음)
	// 프로젝트 : 모듈 + 플러그인 으로 이루어지는 전체 구조
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void SamplesModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();

	UE_LOG(LogTemp, Warning, TEXT("StartUpModule!"));
}

void SamplesModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

// IMPLEMENT_PRIMARY_GAME_MODULE 은 해당 엔진 내에서 하나만 존재해야 함 -> 핵심이 되는 모듈에게만 사용할것
// 모듈마다 이러한 IMPLEMENT 시리즈를 하나씩은 설정해 줘야 함
IMPLEMENT_PRIMARY_GAME_MODULE(SamplesModule, Samples, "Samples" );


