// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;
public:
	void SetServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);
protected:

	UFUNCTION()
		void HostServer();
	UFUNCTION()
		void JoinServer();
	UFUNCTION()
		void QuitPressed();

	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void OpenMainMenu();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancleButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;

private:
	TSubclassOf<UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;
};
