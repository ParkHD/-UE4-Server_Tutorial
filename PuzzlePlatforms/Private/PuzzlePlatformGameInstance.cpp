// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

#include "PlatformTrigger.h"
#include "00_MenuSystem/InGameMenu.h"
#include "00_MenuSystem/MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// 생성자는 에디터 재실행시에도 호출된다.
	UE_LOG(LogTemp, Log, TEXT("Constructor"));

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/BP/00_UMG/WBP_MainMenu"));
	if(MenuBPClass.Class != nullptr)
	{
		MenuClass = MenuBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/BP/00_UMG/WBP_InGameMenu"));
	if (InGameMenuBPClass.Class != nullptr)
	{
		InGameMenuClass = InGameMenuBPClass.Class;
	}
}

void UPuzzlePlatformGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Log, TEXT("Init"));

	UE_LOG(LogTemp, Log, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPuzzlePlatformGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if(Engine != nullptr)
	{
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

		UWorld* World = GetWorld();
		if(World != nullptr)
		{
			World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
		}
	}
}

void UPuzzlePlatformGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (Engine != nullptr)
	{
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green,
			FString::Printf(TEXT("Joining %s"), *Address));

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if(PlayerController != nullptr)
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UPuzzlePlatformGameInstance::LoadMenu()
{
	if(MenuClass != nullptr)
	{
		Menu = CreateWidget<UMainMenu>(this, MenuClass);

		Menu->SetUp();
		Menu->SetMenuInterface(this);
	}
}

void UPuzzlePlatformGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass != nullptr)
	{
		InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
		if(InGameMenu != nullptr)
		{
			InGameMenu->SetUp();
			InGameMenu->SetMenuInterface(this);
		}
	}
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController != nullptr)
	{
		PlayerController->ClientTravel("/Game/BP/00_UMG/MainMenu", ETravelType::TRAVEL_Absolute);
	}
}
