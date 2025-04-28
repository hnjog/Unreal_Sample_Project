#include "SampleHealExecution.h"
#include "Samples/AbilitySystem/Attributes/SampleCombatSet.h"
#include "Samples/AbilitySystem/Attributes/SampleHealthSet.h"

// 해당 Struct를 통해 FGameplayEffectAttributeCaptureDefinition 인스턴스화하여 전달
struct FHealStatics
{
	// AttributeSet의 어떤 Attribute를 어떻게 캡쳐할 것인지에 대한 정의
	// 
	// 클래스 멤버 포인터와도 비슷
	// 특정한 attribute를 지정하고 빠르게 가져올 수 있음
	// (USampleCombatSet::GetBaseHealAttribute())
	// 
	FGameplayEffectAttributeCaptureDefinition BaseHealDef;

	FHealStatics()
	{
		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(USampleCombatSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FHealStatics& HealStatics()
{
	// 계속 FHealStatics를 생성하는 것은 부하가 되므로 생성 후 재사용
	static FHealStatics Statics;
	return Statics;
}

USampleHealExecution::USampleHealExecution()
	:Super()
{
	// Source로 (입력값) Attribute를 캡쳐를 정의
	// - CombatSet::BaseHeal을 통해 Healing 값을 정의
	// 
	// 최종 Execute할 때, 해당 값을 가져와서 Health에 Healing을 적용
	//
	// RelevantAttributesToCapture는 Array이며,
	// GameplayEffect의 계산과정에서 필요한 Attribute를 캡쳐하는 용도로 사용
	// (일일이 데이터를 그떄마다 가져오면 불편하고 느리므로)
	// (계산에 사용할 Attribute를 캡쳐)
	// 
	// EGameplayEffectAttributeCaptureSource
	// - source : 사용자
	// - target : 적용자 
	// 와 비슷 
	// (ex : 공격을 한다 치면 공격자의 공격력을 가져와야 함 -> 이때는 source,
	// 방어하는 사람의 방어력은 Target이 된다)
	//
	RelevantAttributesToCapture.Add(HealStatics().BaseHealDef);
}

void USampleHealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// GameplayEffectSpec은 GameplayEffect의 핸들
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float BaseHeal = 0.0f;
	{
		FAggregatorEvaluateParameters EvaluateParameters;

		// 해당 함수 호출을 통해 CombatSet의 BaseHeal 값을 가져옴
		// AttemptCalculateCapturedAttributeMagnitude 를 통해
		// USampleCombatSet 의 BaseHeal 을 가져오고
		// (이 때, Modifier에 누적되어 있었다면 그 계산 결과를 가져옴
		// -> 변화값이 적용)
		// BaseHeal 에 저장
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().BaseHealDef, EvaluateParameters, BaseHeal);
	}

	// RelevantAttributesCapture을 통해 최종 계산된 BaseHeal을 0.0이하가 되지 않도록(- 되면 체력이 깎이므로)
	const float HealingDone = FMath::Max(0.0f, BaseHeal);
	if (HealingDone > 0.0f)
	{
		// GameplayEffectCalculation 이후, Modifier로서, 추가
		// - 해당 Modifier는 CombatSet에서 가져온 BaseHeal을 활용하여, HealthSet의 Healing에 추가
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USampleHealthSet::GetHealingAttribute(), EGameplayModOp::Additive, HealingDone));
	}
}
