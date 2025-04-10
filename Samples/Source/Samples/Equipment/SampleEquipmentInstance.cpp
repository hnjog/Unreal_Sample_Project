#include "SampleEquipmentInstance.h"
#include "Samples/Equipment/SampleEquipmentDefinition.h"
#include "GameFramework/Character.h"

USampleEquipmentInstance::USampleEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

APawn* USampleEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void USampleEquipmentInstance::SpawnEquipmentActors(const TArray<FSampleEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		// 현재 Owner인 Pawn의 RootComponent를 AttachTarget 대상으로 한다
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			// 만약 캐릭터라면, SkeletalMeshComponent가 있으면 GetMesh로 반환하여, 여기에 붙임
			// 그렇지 않으면 Animation이 제대로 적용되지 않을 수 있음
			AttachTarget = Char->GetMesh();
		}

		for (const FSampleEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			// SpawnActorDeferred는 FinishSpawning을 호출해야 Spawn이 완성
			// 데이터만 세팅해두고, 필요 시점에 실제 Spawn
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/true);

			// Actor의 RelativeTransform을 AttachTransform으로 설정
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

			// AttachTarget에 붙이자 (Actor -> Actor)
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void USampleEquipmentInstance::DestroyEquipmentActors()
{
	// Actor가 복수개의 Mesh로 구성되어 있을수 있으므로 저눕 파괴
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

APawn* USampleEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void USampleEquipmentInstance::OnEquipped()
{
	// 이러한 효과는 BP에서 작성하도록
	// 애니메이션 로직은 BP가 편하기에 이런듯?
	K2_OnEquipped();
}

void USampleEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
