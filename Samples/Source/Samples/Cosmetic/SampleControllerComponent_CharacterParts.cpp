#include "SampleControllerComponent_CharacterParts.h"
#include "SamplePawnComponent_CharacterParts.h"

USampleControllerComponent_CharacterParts::USampleControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void USampleControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			// OnPossessedPawnChanged : Possess하고 있는 Pawn이 변경될때마다
			// 등록된 함수를 호출하는 Delegate
			// 
			// 컨트롤러가 Possess 될때 OnPossessedPawnChanged 를 호출
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void USampleControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

PRAGMA_DISABLE_OPTIMIZATION
USamplePawnComponent_CharacterParts* USampleControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	// Controller가 Possess 되지 않은 경우 불발됨
	// PlayerController의 생성(Beginplay 호출)과 Possess 시점과 '다를 수 있음'
	// 언제 possess 되는지 정확히 알순 없음
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 폰 컴포넌트를 가지는 BP를 이미 추가 
		// 컴포넌트 방식의 장점, FindComponentByClass로 편리하게 가져올 수 있음
		return ControlledPawn->FindComponentByClass<USamplePawnComponent_CharacterParts>();
	}
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

// 이게 호출됨으로서 시작
void USampleControllerComponent_CharacterParts::AddCharacterPart(const FSampleCharacterPart& NewPart)
{
	// Dedicate 구조..?
	AddCharacterPartInternal(NewPart);
}

void USampleControllerComponent_CharacterParts::AddCharacterPartInternal(const FSampleCharacterPart& NewPart)
{
	// AddDefaulted_GetRef
	// 내부적으로 요소의 크기만큼 할당한 후, 그 포인터를 넘겨줌 (일종의 팩토리식 함수)
	FSampleControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	// Parts를 실제로 Spawn 및 달아주는건 Pawn 쪽에서 할 일
	if (USamplePawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void USampleControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (USamplePawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FSampleControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}

void USampleControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// old pawn 에서 Character Part 제거
	// PawnComponent를 찾아서 진행한다
	if (USamplePawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<USamplePawnComponent_CharacterParts>() : nullptr)
	{
		for (FSampleControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// new pawn에 기존 Character Part 추가
	if (USamplePawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<USamplePawnComponent_CharacterParts>() : nullptr)
	{
		for (FSampleControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}

