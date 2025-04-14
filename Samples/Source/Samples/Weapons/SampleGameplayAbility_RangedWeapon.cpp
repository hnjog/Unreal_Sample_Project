#include "SampleGameplayAbility_RangedWeapon.h"
#include "SampleRangedWeaponInstance.h"
#include "Samples/AbilitySystem/SampleAbilitySystemComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "CollisionShape.h"
#include "Samples/Physics/SampleCollisionChannels.h"
#include "Samples/AbilitySystem/SampleGameplayAbilityTargetData_SingleTarget.h"

USampleGameplayAbility_RangedWeapon::USampleGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USampleGameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
	// ActorInfo는 AbilitySet에서 GiveAbility() 호출로 설정된다
	// - UGameplayAbility::OnGiveAbility()에서 SetCurrentActorInfo()에서 설정된다
	// - AbilitySystemComponent::GiveAbility()에서 OnGiveAbility() 호출한다
	// - SampleAbilitySet::GiveToAbilitySystem()에서 GiveAbility()를 호출한다
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	// 여기서 Lyra는 샷건 처리와 같은 탄착 처리를 생략하고, 권총으로 진행

	// 총알의 궤적의 Hit 정보를 계산
	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);

	// GameplayAbilityTargetData는 Server/Client 간 Ability의 공유 데이터로 이해
	// - 허나, 우리는 싱글플레이이므로 Ability의 데이터로 생각 (현재 큰 의미가 없음)
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = 0;

	if (FoundHits.Num() > 0)
	{
		// Cartridge란 일반 권총의 경우, 탄약에 하나의 총알이 들어있지만, 
		// 샷견의 경우, 탄약에 여러개의 총알이 있고, **탄약을 카트리지로 생각**
		const int32 CartridgeID = FMath::Rand();
		for (const FHitResult& FoundHit : FoundHits)
		{
			// AbilityTargetData에 SingeTargetHit 정보를 담는다
			// - 참고로 TargetData.Add()의 경우, SharedPtr에 넣기 때문에 여기서 new는 크게 신경 안써도 된다
			FSampleGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FSampleGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
			TargetData.Add(NewTargetData);
		}
	}

	// 가공된 AbilityTargetData가 준비되었으므로, OnTargetDataReadyCallback을 호출
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}

void USampleGameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	USampleRangedWeaponInstance* WeaponData = GetWeaponInstance();
	if (AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData)
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = WeaponData;
		InputData.bCanPlayBulletFX = true;

		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn, ESampleAbilityTargetingSource::CameraTowardsFocus);
		// 언리얼은 ForwardVector가 (1, 0, 0) 즉 EAxis::X이다
		// - GetUnitAxis()를 살펴보자
		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();
		InputData.EndAim = InputData.StartTrace + InputData.AimDir * WeaponData->MaxDamageRange;

#if 1
		{
			static float DebugThickness = 2.0f;
			DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.0f), FColor::Yellow, false, 10.0f, 0, DebugThickness);
		}
#endif

		TraceBulletsInCartridge(InputData, OutHits);
	}
}

FTransform USampleGameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, ESampleAbilityTargetingSource Source)
{
	check(SourcePawn);
	check(Source == ESampleAbilityTargetingSource::CameraTowardsFocus);

	// 참고로 아래 로직은 CameraTowardsFocus만 추출
	// - 완전한 로직은 Lyra를 참고

	AController* Controller = SourcePawn->Controller;
	if (Controller == nullptr)
	{
		return FTransform();
	}

	// 매직넘버...
	double FocalDistance = 1024.0f;
	FVector FocalLoc;
	FVector CamLoc;
	FRotator CamRot;

	// PlayerController로부터, Location과 Rotation 정보를 가져옴
	APlayerController* PC = Cast<APlayerController>(Controller);
	check(PC);
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	FVector AimDir = CamRot.Vector().GetSafeNormal();
	FocalLoc = CamLoc + (AimDir * FocalDistance);

	// WeaponLoc이 아닌 Pawn의 Loc
	const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
	//(WeaponLoc - FocalLoc) | AimDir : Operator| 에 의하여 Dot(외적) 실행
	// WeaponLoc : pawn의 위치
	// FocalLoc = 카메라 위치 에서 에임 방향 * 거리 를 통해 구한 위치
	FVector FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);

#if 1
	{
		// WeaponLoc (사실상 ActorLoc)
		DrawDebugPoint(GetWorld(), WeaponLoc, 10.0f, FColor::Red, false, 60.0f);
		// CamLoc
		DrawDebugPoint(GetWorld(), CamLoc, 10.0f, FColor::Yellow, false, 60.0f);
		// FinalCamLoc
		DrawDebugPoint(GetWorld(), FinalCamLoc, 10.0f, FColor::Magenta, false, 60.0f);

		// (WeaponLoc - FocalLoc)
		DrawDebugLine(GetWorld(), FocalLoc, WeaponLoc, FColor::Yellow, false, 60.0f, 0, 2.0f);
		// (AimDir)
		DrawDebugLine(GetWorld(), CamLoc, FocalLoc, FColor::Blue, false, 60.0f, 0, 2.0f);

		// Project Direction Line
		DrawDebugLine(GetWorld(), WeaponLoc, FinalCamLoc, FColor::Red, false, 60.0f, 0, 2.0f);
	}
#endif

	// Camera -> Focus 계산 완료
	return FTransform(CamRot, FinalCamLoc);
}

FVector USampleGameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	// 그냥 Pawn의 위치를 가져옴(왜지?)
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	const FVector SourceLoc = AvatarPawn->GetActorLocation();
	return SourceLoc;
}

void USampleGameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits)
{
	USampleRangedWeaponInstance* WeaponData = InputData.WeaponData;
	check(WeaponData);

	// MaxDamageRange를 고려하여, EndTrace를 정의하자
	const FVector BulletDir = InputData.AimDir;
	const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->MaxDamageRange);

	// HitLocation의 초기화 값으로 EndTrace로 설정
	FVector HitLocation = EndTrace;

	// 총알을 하나 Trace 진행한다
	// - 참고로 Lyra의 경우, 샷건과 같은 Cartridge에 여러개의 총알이 있을 경우를 처리하기 위해 for-loop을 활용하여, 복수개 Bullet을 Trace한다
	TArray<FHitResult> AllImpacts;
	FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, EndTrace, WeaponData->BulletTraceWeaponRadius, /**bIsSimulated=*/ false, /**out*/ AllImpacts);

	const AActor* HitActor = Impact.GetActor();
	if (HitActor)
	{
		if (AllImpacts.Num() > 0)
		{
			OutHits.Append(AllImpacts);
		}

		HitLocation = Impact.ImpactPoint;
	}

	// OutHits가 적어도 하나가 존재하도록, EndTrace를 활용하여, OutHits에 추가해준다
	if (OutHits.Num() == 0)
	{
		if (!Impact.bBlockingHit)
		{
			Impact.Location = EndTrace;
			Impact.ImpactPoint = EndTrace;
		}

		OutHits.Add(Impact);
	}
}

// 클래스 함수가 아닌 일반 함수임
int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Idx;
		}
		else // 걸린 것이 없음
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();

			// 장비 아이템(헬멧, 장갑 등)? 이 맞은 경우?
			// 한단계 AttachParent에 Actor가 Pawn이라면?
			// - 보통 복수개 단계로 AttachParent를 하지 않으므로, AttachParent 대상이 APawn이라고 생각할 수도 있겠다
			// (타고타고 안 올라가는 이유는 잘 모르겠다 - 너무 무겁거나, 판정 버그를 유의?)
			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) 
				&& (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				return Idx;
			}
		}
	}
	return INDEX_NONE;
}

FHitResult USampleGameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const
{
	FHitResult Impact;

	// 우선 SweepRadius 없이 한번 Trace 진행
	// (SweepTrace는 무겁기 때문)
	// (그렇기에 0.0 으로 line 을 쏜 후,
	//  그래도 부딪히지 않으면 Radius를 주고 탐색)
	// (사실 LineTrace도 무거운 연산)
	// 
	// - FindFirstPawnHitResult()를 여러번 Trace 진행을 막기 위해, 
	// OutHits를 확인해서 APawn 충돌 정보있으면 더이상 Trace하지 않는다
	// 
	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		Impact = WeaponTrace(StartTrace, EndTrace, /*SweepRadius=*/0.0f, bIsSimulated, /*out*/ OutHits);
	}

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		// 만약 SweepRadius가 0보다 크면, 0.0일때 대비 충돌 가능성이 커지므로 한번 더 Trace 진행
		if (SweepRadius > 0.0f)
		{
			// SweepHits에 Trace의 OutHits 정보를 저장
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, SweepHits);

			// Sphere Trace로 진행한 결과인 SweepHits를 검색하여, Pawn이 있는가 검색
			const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);
			if (SweepHits.IsValidIndex(FirstPawnIdx))
			{
				// 만약 있다면, SweepHits를 FirstPawnIdx까지 순회하며, bBlockingHit와 기존 OutHits에 없을 경우 체크한다
				bool bUseSweepHits = true;
				for (int32 Idx = 0; Idx < FirstPawnIdx; ++Idx)
				{
					const FHitResult& CurHitResult = SweepHits[Idx];

					auto Pred = [&CurHitResult](const FHitResult& Other)
						{
							return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
						};

					// OutHits에 있다면... SweepHits를 OutHits로 업데이트 하지 않는다 (이미 충돌했던 정보가 있으니깐?) (early-out용)
					// - OutHits에 bBlockingHits가 SweepHits로 있음을 알게되었음
					if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
					{
						bUseSweepHits = false;
						break;
					}
				}

				// SweepHits
				if (bUseSweepHits)
				{
					OutHits = SweepHits;
				}
			}
		}
	}

	return Impact;
}

FHitResult USampleGameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;

	// Complex Geometry로 Trace를 진행하며, AvatarActor를 AttachParent를 가지는 오브젝트와의 충돌은 무시
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex*/true, /*IgnoreActor=*/GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;

	// AvatarActor에 부착된 Actors를 찾아 IgnoredActors에 추가한다
	AddAdditionalTraceIgnoreActors(TraceParams);

	// Weapon 관련 Collision Channel로 Trace 진행
	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);
	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (HitResults.Num() > 0)
	{
		// HitResults 중에 중복(같은) Object의 HitResult 정보를 제거
		for (FHitResult& CurHitResult : HitResults)
		{
			auto Pred = [&CurHitResult](const FHitResult& Other)
				{
					return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
				};

			if (!OutHitResults.ContainsByPredicate(Pred))
			{
				OutHitResults.Add(CurHitResult);
			}
		}

		// Hit의 가장 마지막 값을 Impact로 저장
		Hit = OutHitResults.Last();
	}
	else
	{
		// Hit의 결과 값을 기본 값으로 캐싱
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}

	return Hit;
}

ECollisionChannel USampleGameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const
{
	return Sample_TraceChannel_Weapon;
}

void USampleGameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	// 붙어 있는 액터들을 제외하기 위하여
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;

		// GetAttachedActors를 한번 보자:
		// - 해당 함수는 Recursively하게 모든 Actors를 추출
		// - 근데 왜 FindFirstPawnHitResult 쪽에선 왜 한단계만?
		Avatar->GetAttachedActors(AttachedActors);

		TraceParams.AddIgnoredActors(AttachedActors);
	}
}

void USampleGameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilitySystemComponent);

	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		// 현재 Stack에서 InData에서 지금 Local로 Ownership을 가져온다
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		// CommitAbility 호출로 GE(GameplayEffect)를 처리한다
		// - 현재 아직 우리는 GE에 대해 처리를 진행하지 않을 것이다
		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			// OnRangeWeaponTargetDataReady BP 노드 호출
			// - 나중에 GCN(GameplayCueNotify)를 처리할 것이다
			OnRangeWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			// CommitAbility가 실패하였으면, EndAbility BP Node 호출한다
			K2_EndAbility();
		}
	}
}

USampleRangedWeaponInstance* USampleGameplayAbility_RangedWeapon::GetWeaponInstance()
{
	return Cast<USampleRangedWeaponInstance>(GetAssociatedEquipment());
}
