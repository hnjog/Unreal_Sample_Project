#pragma once

#include "Containers/Array.h"
#include "Templates/SubclassOf.h"
#include "Samples/Inventory/SampleInventoryItemDefinition.h"
#include "SampleInventoryFragment_ReticleConfig.generated.h"

class USampleReticleWidgetBase;

UCLASS()
class USampleInventoryFragment_ReticleConfig : public USampleInventoryItemFragment
{
	GENERATED_BODY()
public:
	// 무기에 결합된 ReticleWidget 정보를 가지고 있는 Fragment
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
	TArray<TSubclassOf<USampleReticleWidgetBase>> ReticleWidgets;
};
