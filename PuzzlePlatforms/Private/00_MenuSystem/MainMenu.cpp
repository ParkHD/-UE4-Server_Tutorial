// Fill out your copyright notice in the Description page of Project Settings.


#include "00_MenuSystem/MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"



bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
		return false;

	HostButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OpenJoinMenu);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::QuitPressed);

	CancleButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OpenMainMenu);
	ConfirmJoinMenuButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::JoinServer);

	return true;
}


void UMainMenu::HostServer()
{
	if(MenuInterface != nullptr)
	{
		MenuInterface->Host();
		UE_LOG(LogTemp, Log, TEXT("HostServer"));
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		FString Address = EditableTextBox_Address->GetText().ToString();
		MenuInterface->Join(Address);
		UE_LOG(LogTemp, Log, TEXT("JoinServer"));
	}
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController != nullptr)
		{
			PlayerController->ConsoleCommand("Quit");
		}
	}
}

void UMainMenu::OpenJoinMenu()
{
	if(MenuSwitcher != nullptr)
	{
		if(JoinMenu != nullptr)
		{
			MenuSwitcher->SetActiveWidget(JoinMenu);
			UE_LOG(LogTemp, Log, TEXT("JoinServer"));
		}
	}
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher != nullptr)
	{
		if (JoinMenu != nullptr)
		{
			MenuSwitcher->SetActiveWidget(MainMenu);
		}
	}
}
