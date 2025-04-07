#pragma once

#include "CoreMinimal.h"
#include "SampleCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FSampleCharacterPartHandle
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
	
public:
	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

};


USTRUCT(BlueprintType)
struct FSampleCharacterPart
{
	GENERATED_BODY()
public:
	// 파츠로 생성된 Actor의 class 정보를 가짐
	// (Actor bp 가 하나의 파츠)
	// Ex) 총을 쏜다던가, 전사라던가 그런 것을 BP클래스로 만든후 여기에 넣을 예정
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	// Actor를 소환하여 붙이는데, 특정 Bone에 붙이기 위한 정보
	// 따로 정의되어 있지 않으면 Root에 붙임
	FName SocketName;
};
