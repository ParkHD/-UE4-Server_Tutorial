// Fill out your copyright notice in the Description page of Project Settings.


#include "00_MenuSystem/InGameMenu.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
		return false;

	CancleButton->OnClicked.AddUniqueDynamic(this, &UInGameMenu::CanclePressed);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UInGameMenu::QuitPressed);


	return true;
}

void UInGameMenu::CanclePressed()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController != nullptr)
		{
			bIsFocusable = true;
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UInGameMenu::QuitPressed()
{
	if(MenuInterface != nullptr)
	{
		MenuInterface->LoadMainMenu();
	}
}
