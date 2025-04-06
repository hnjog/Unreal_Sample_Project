#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.generated.h"

// GFCM에 '등록'하기 위한 용도의 Wrapper 클래스
// 이걸 상속받음으로서 GFCM에 Receiver가 될 수 있도록 처리 
UCLASS()
class MODULARGAMEPLAYACTORS_API AModularCharacter : public ACharacter
{
	GENERATED_BODY()

	// Character Interface
public:
	// 기본적인 초기화 시점에 호출(BeginPlay 이전)
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
