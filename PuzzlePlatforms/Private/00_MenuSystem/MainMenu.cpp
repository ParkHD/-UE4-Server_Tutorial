// Fill out your copyright notice in the Description page of Project Settings.


#include "00_MenuSystem/MainMenu.h"

#include "00_MenuSystem/ServerRow.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/BP/00_UMG/WBP_ServerRow"));
	if (ServerRowBPClass.Class != nullptr)
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
		return false;

	HostButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OpenHostMenu);
	JoinButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OpenJoinMenu);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::QuitPressed);

	CancleButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OpenMainMenu);
	ConfirmJoinMenuButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::JoinServer);

	ConfirmHostMenuButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::HostServer);
	CancleHostMenuButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::SetServerList(TArray<FServerData> ServerDatas)
{
	UWorld* World = this->GetWorld();
	if (World != nullptr)
	{
		ServerList->ClearChildren();

		uint32 i = 0;
		for(const FServerData& ServerData : ServerDatas)
		{
			auto ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
			ServerRow->ServerName->SetText(FText::FromString(ServerData.Name));
			ServerRow->HostUser->SetText(FText::FromString(ServerData.HostUsername));
			FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
			ServerRow->ConnectionPraction->SetText(FText::FromString(FractionText));

			ServerRow->Setup(this, i);
			i++;
			ServerList->AddChild(ServerRow);
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}


void UMainMenu::HostServer()
{
	if(MenuInterface != nullptr)
	{
		FString ServerName = ServerHostName->GetText().ToString();
		MenuInterface->Host(ServerName);
		UE_LOG(LogTemp, Log, TEXT("HostServer"));
	}
}

void UMainMenu::JoinServer()
{
	if(SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		MenuInterface->Join(SelectedIndex.GetValue());
		UE_LOG(LogTemp, Log, TEXT("Selected Index : %d"), SelectedIndex.GetValue());
	}
	else
	{
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
			if(MenuInterface != nullptr)
			{
				MenuInterface->RefreshServerList();
			}
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

void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher != nullptr)
	{
		if (HostMenu != nullptr)
		{
			MenuSwitcher->SetActiveWidget(HostMenu);
		}
	}
}

void UMainMenu::UpdateChildren()
{
	for(int i = 0;i<ServerList->GetChildrenCount();i++)
	{
		auto row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if(row != nullptr)
		{
			row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
