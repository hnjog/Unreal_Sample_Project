#include "SamplePawnComponent_CharacterParts.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"

bool FSampleCharacterPartList::SpawnActorForEntry(FSampleAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;
	// 전달된 AppliedCharacterPartEntry의 Part Class 체크
	if (Entry.Part.PartClass != nullptr)
	{
		// OwnerComponent의 Owner에 속한 World를 찾음
		UWorld* World = OwnerComponent->GetWorld();

		// Pawn Component에 어느 Component에 붙일 것인지 결정
		// - GetSceneComponentToAttachTo
		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			// 붙일 Component인 ComponentToAttachTo의 Bone 혹은 SocketName을 통해 어디에 붙일지 Transform을 계산
			// 만들어져 있지만 사용은 안함(일반적으로 root에 붙이는 편 - lyra에서도 사용은 안하는)
			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			// Actor-Actor의 결합이므로, ChildActorComponent를 활용
			// OwnerComponent->GetOwner() : 현재 빙의한 폰
			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);

			// SetChildActorClass 를 통해 세팅 후 아래의 RegisterComponent 호출
			PartComponent->SetChildActorClass(Entry.Part.PartClass);

			// RegisterComponent를 통해 마지막으로 RenderWorld인 FScene에 변경 내용을 전달
			// (혹은 생성)
			// -> 렌더링에 반영 및 Spawn
			// 
			// Data 용 World, Render 용 World -> 멀티스레드 환경
			// Register를 통해 이들에게 등록(OnRegister)
			PartComponent->RegisterComponent();

			// ChildActorComponent에서 생성한 Actor를 반환
			// (Entry.Part.PartClass 로 생성된 객체)
			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				// 해당 Actor가 Parent인 Pawn Component의 Owner Actor보다 먼저 Tick이 실행되지 않도록 선행조건을 붙임
				if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
				{
					// 스폰시킨 액터의 Tick이 먼저 발생하는 경우를 방지하는 용
					SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
				}
			}

			// 스폰한 UChildActorComponent 등록
			Entry.SpawnedComponent = PartComponent;
			bCreatedAnyActor = true;
		}
	}

	return bCreatedAnyActor;

}

bool FSampleCharacterPartList::DestroyActorForEntry(FSampleAppliedCharacterPartEntry& Entry)
{
	bool bDestroyedAnyActors = false;

	if (Entry.SpawnedComponent)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
		bDestroyedAnyActors = true;
	}

	return bDestroyedAnyActors;
}

FSampleCharacterPartHandle FSampleCharacterPartList::AddEntry(FSampleCharacterPart NewPart)
{
	// PawnComponent의 CharacterPartList가 PartHandle을 관리
	// 이를 ControllerComponent_CharacterParts에 전달
	FSampleCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++; // 다음 녀석과 겹치지 않도록

	// Authority가 있다면, AppliedCharacterPartEntry를 Entries에 추가
	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		// Entries 에서 레퍼런스로 받은것이기에
		// 결국 Entries에서 관리 가능
		FSampleAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart; // 메타데이터 (레퍼런스로 안 받은 이유)
		NewEntry.PartHandle = Result.PartHandle;

		// 여기서 실제 Actor 생성
		// OwnerComponent의 Owner Actor에 Actor 끼리 RootComponent로 Attach
		if (SpawnActorForEntry(NewEntry))
		{
			// BroadcastChanged를 통해
			// OwnerComponent에서 Owner의 SkeletalMeshComponent 활용
			// Animation 및 Physics를 Re-initialize 시킴
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}

void FSampleCharacterPartList::RemoveEntry(FSampleCharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; EntryIt++)
	{
		FSampleAppliedCharacterPartEntry& Entry = *EntryIt;

		if (Entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(Entry);
			break; 
		}
	}
}

FGameplayTagContainer FSampleCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	// Entries 순회
	for (const FSampleAppliedCharacterPartEntry& Entry : Entries)
	{
		// Part Actor가 생성되어 Spawned Component에 캐싱되어 있다면
		if (Entry.SpawnedComponent)
		{
			// 해당 Actor의 IGameplayTagAssetInterface를 통해 GameplayTag 검색
			// (당장은 IGameplayTagAssetInterface를 상속받지 않지만)
			// (나중에 일부 'part'에 gameplaytag를 넣고 싶다면 상속받아야 함)
			// (ex -> 특정 조건에 장착 가능한 장비?)
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(Result);
			}
		}
	}
	return Result;
}

USamplePawnComponent_CharacterParts::USamplePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	CharacterPartList(this)
{

}

USkeletalMeshComponent* USamplePawnComponent_CharacterParts::GetParentMeshComponent() const
{
	// Character를 활용하여 최상위 SkeletalMesh 반환
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
				return MeshComponent;
		}
	}
	return nullptr;
}

USceneComponent* USamplePawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	// Parent에 SkeletalMeshComponent가 있으면 반환
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
		return MeshComponent;

	// 아니면 RootComponent 확인
	if (AActor* OwnerActor = GetOwner())
		return OwnerActor->GetRootComponent();

	return nullptr;
}

FGameplayTagContainer USamplePawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	// 장착된 CharacterPartList 의 Merge 된 Tags를 반환
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		// FGameplayTag 를 통해 필터링 하는 경우는 그대로 진행
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}

	return Result;
}

void USamplePawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	// 현재 Owner의 SkeletalMeshComponent를 반환
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// BodyMeshes를 통해 GameplayTag를 활용
		// 알맞은 SkeletalMesh로 재설정
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

		// 스켈레탈 메쉬 및 애니메이션 초기화
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

		// physicsasset 초기화
		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			// 생각보단 무거운 작업
			// -> 똑같은 PhysicsAsset 인 경우는 재수행하진 않는다
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}
}

FSampleCharacterPartHandle USamplePawnComponent_CharacterParts::AddCharacterPart(const FSampleCharacterPart& NewPart)
{
	// 일단 관리하고 있는 리스트에 넣어주면서 일 진행
	return CharacterPartList.AddEntry(NewPart);
}

void USamplePawnComponent_CharacterParts::RemoveCharacterPart(FSampleCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}


