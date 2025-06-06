﻿#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "SampleCharacterPartTypes.h"
#include "SampleControllerComponent_CharacterParts.generated.h"

class USamplePawnComponent_CharacterParts;

// Controller Component가 소유하는 Character Parts
USTRUCT()
struct FSampleControllerCharacterPartEntry
{
	GENERATED_BODY()
public:
	// Character Part에 대한 정의 (메타데이터 == MetaData)
	UPROPERTY(EditAnywhere)
	FSampleCharacterPart Part;

	// Character Part 핸들(고유값) - Controller가 Possess하고 있는 Pawn에서 생성한 (인스턴스) Character Part 핸들값
	FSampleCharacterPartHandle Handle;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class USampleControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	USampleControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	USamplePawnComponent_CharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FSampleCharacterPart& NewPart);
	void AddCharacterPartInternal(const FSampleCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
public:
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FSampleControllerCharacterPartEntry> CharacterParts;
};
