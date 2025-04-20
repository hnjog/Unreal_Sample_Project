#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "PrimaryGameLayout.generated.h"

UCLASS(Abstract)
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	
};

/*
	가장 큰 레이아웃(하나만 존재)

	이 내부에 다양한 UI를 세팅하기에,
	제일 먼저 등록되어야 한다
*/