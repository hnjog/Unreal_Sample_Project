#include "SampleExperienceManagerComponent.h"
#include "../System/SampleAssetManager.h"
#include "../GameModes/SampleExperienceDefinition.h"

void USampleExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnSampleExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		// 요청하는 함수에게 이미 로딩이 되었으므로
		// 현재 Experience를 인자로 건네주며 실행시킨다(callback)
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		// movetemp 를 통해 rvalue Reference 위치를 이동 시킴
		// Delegate 리스트에 해당 함수를 등록
		//
		// Delegate 객체는 내부적으로 필요한 변수를 메모리 할당해놓음
		// (함수 인자 등, 여러 상태를 저장하는 변수
		// 
		// ex)
		// TArray<int> a = {1,2,3,4}
		// delegate_type delegate = [a](){return a.num;}
		// -> a는 delegate_type 내부에 new로 할당이 된다
		//  -> 복사 비용을 낮추기 위하여 Move를 통하여 전달
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void USampleExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	USampleAssetManager& AssetManager = USampleAssetManager::Get();

	TSubclassOf<USampleExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad()); // 동기식 load
	}

	// CDO : Class Default Object
	// 특정한 클래스의 기본 인스턴스
	// -> 해당 클래스의 기본 속성값을 가진 객체
	// (CDO의 값을 변경하는 경우, 차후 생성되는 인스턴스의 기본값이 변경됨)
	// 
	// CDO 를 가져오는 이유?
	const USampleExperienceDefinition* Experience = GetDefault<USampleExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// CDO로 CurrentExperience를 설정
		// Experience의 CDO를 변경했다면 CurrentExperience에 변경점도 같이 적용된다?
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void USampleExperienceManagerComponent::StartExperienceLoad()
{
}
