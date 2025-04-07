#include "SampleControllerComponent_CharacterParts.h"
#include "SamplePawnComponent_CharacterParts.h"

USampleControllerComponent_CharacterParts::USampleControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

PRAGMA_DISABLE_OPTIMIZATION
USamplePawnComponent_CharacterParts* USampleControllerComponent_CharacterParts::GetPawnCustomizer() const
{
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

