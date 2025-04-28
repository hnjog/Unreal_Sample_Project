#include "SampleHealthSet.h"

USampleHealthSet::USampleHealthSet()
	:Super(),
	Health(50),
	MaxHealth(100)
{
}

void USampleHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// 현재 체력이라면 0 ~ 최대 체력
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// 최대 체력이라면 1 보단 크도록
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void USampleHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void USampleHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}
