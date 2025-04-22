#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

class UGameUIPolicy;
class UCommonLocalPlayer;

UCLASS(Abstract, Config = Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGameUIManagerSubsystem();

public:
	// Member Method
	void SwitchToPolicy(UGameUIPolicy* InPolicy);

	// 3코드 모두 단순히 Policy에 넘겨주는(책임전가) 하는 코드
	// -> 그냥 이 안에서 처리하면 안되나??
	//    (명색이 Manager인데)
	// 
	// => 코드의 구조가 커지면 해당 함수를 override하기 위하여
	//    (기능이나 로직 등을 바꾸기 위해)
	//	  UGameUIManagerSubsystem 를 상속받기가 조금 '무거워'지며,
	//	  다소는 불필요한 기능까지 들고 가야 한다
	//	  (또한, 이러한 Subsystem을 계속 상속받다보면, '마지막' 클래스만 사용해야 함
	//     => 차후 해당 클래스 제거 시, 이전 상속된 클래스로 돌아가는 것은 생각보다 경직적인 구조)
	// 
	// 반대로 현재 방식은
	// UGameUIPolicy 를 상속받아 해당 클래스를 상속받으면
	// 더 직관적이고 관리가 쉬워진다
	// (해당 데이터를 든 Policy에서, 상속받아 함수만 바꿔주면 되니까)
	// (제거하더라도, 해당 Policy 상속 받은 녀석만 제거하면 되므로 구조가 더 유연하다)
	// 
	// UGameUIPolicy 라는 실질적인 '데이터 관리' 용 클래스를 별도로 만들어
	// 차후 해당 기능에 대한 처리를 따로 뺌으로서,
	// 확장성을 고려
	// (반대로 처음에 간단히 만들어 두고, 나중에 이런식으로 확장이 가능하단 것도 알아는 두자)
	// (처음부터 이 방식을 사용하는 경우도 나쁘진 않지만, '복잡'해질 수는 있음)
	//
	virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

public:
	// UGameInstanceSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

	// defalut UI Policy를 생성할 class
	UPROPERTY(Config,EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};
