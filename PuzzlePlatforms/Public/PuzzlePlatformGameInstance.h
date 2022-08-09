// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "00_MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	// InGame 콘솔에서 입력가능한 명령어
	UFUNCTION(Exec)
		virtual void Host() override;
	UFUNCTION(Exec)
		virtual void Join(uint32 Index) override;
	virtual void RefreshServerList() override;

	UFUNCTION(BlueprintCallable)
		void LoadMenu();
	UFUNCTION(BlueprintCallable)
		void LoadInGameMenu();
private:
	virtual void LoadMainMenu() override;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
private:
	TSubclassOf<UUserWidget> MenuClass;
	TSubclassOf<UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;
	class UInGameMenu* InGameMenu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};

