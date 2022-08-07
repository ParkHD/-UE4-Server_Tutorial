// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	// InGame 콘솔에서 입력가능한 명령어
	UFUNCTION(Exec)
		void Host();
	UFUNCTION(Exec)
		void Join(const FString& Address);
};
