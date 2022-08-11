 // Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

#include "PlatformTrigger.h"
#include "00_MenuSystem/InGameMenu.h"
#include "00_MenuSystem/MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("My Seesion Name");
const static FName SERVER_NAME_SETTING_KEY = TEXT("Server Name");

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

	// IOnlineSubsystem은 싱글톤 클래스이다.
	IOnlineSubsystem* SubSystem =  IOnlineSubsystem::Get();
	if(SubSystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found SubSystem %s"), *SubSystem->GetSubsystemName().ToString());
		SessionInterface = SubSystem->GetSessionInterface();
		if(SessionInterface != nullptr)
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Init"));
	UE_LOG(LogTemp, Log, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPuzzlePlatformGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if(SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if(ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::Join(uint32 Index)
{

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if(Menu != nullptr)
	{
	}
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

}

 void UPuzzlePlatformGameInstance::RefreshServerList()
 {
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
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

 void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
 {
	if(!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create session"));
		return;
	}
	 UEngine* Engine = GetEngine();
	 if (Engine != nullptr)
	 {
		 Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

		 UWorld* World = GetWorld();
		 if (World != nullptr)
		 {
			 World->ServerTravel("/Game/BP/Maps/Lobby?listen");
		 }
	 }
 }
 void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
 {
	if(Success)
	{
		CreateSession();
	}
 }

 void UPuzzlePlatformGameInstance::OnFindSessionComplete(bool Success)
 {
	UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));
	if(Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> ServerDatas;
		for(const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			FServerData ServerData;
			ServerData.Name = SearchResult.GetSessionIdStr();
			// null SubSystem에서는 공개 연결수를 제대로 측정을 못한다. Steam에서는 제대로 된 값을 얻을 수 있음
			ServerData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = ServerData.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			ServerData.HostUsername = SearchResult.Session.OwningUserName;

			FString ServerName;
			if(SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTING_KEY, ServerName))
			{
				ServerData.Name = ServerName;
			}
			else
			{
				ServerData.Name = "Could not find Name";
			}

			ServerDatas.Add(ServerData);

			UE_LOG(LogTemp, Warning, TEXT("Finished Find Session : %s"), *SearchResult.GetSessionIdStr());
		}
		Menu->SetServerList(ServerDatas);
	}
 }
 void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName,
	 EOnJoinSessionCompleteResult::Type Result)
 {
	if (!SessionInterface.IsValid()) return;
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}
	UEngine* Engine = GetEngine();
	if (Engine != nullptr)
	{
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green,
			FString::Printf(TEXT("SessionName : %s"), *SessionName.ToString()));
		//Engine->AddOnScreenDebugMessage(0, 5, FColor::Green,
		//	FString::Printf(TEXT("Joining %s"), *Address));

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if(PlayerController != nullptr)
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
 }

 void UPuzzlePlatformGameInstance::CreateSession()
 {
	if(SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
			SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = 2;		// 연결 플레이어 수
		SessionSettings.bShouldAdvertise = true;		// 온라인에서 세션을 볼 수 있게. 초대
		SessionSettings.bUsesPresence = true;			// 스팀에서 로비를 사용하도록
		SessionSettings.bUseLobbiesIfAvailable = true;	// 스팀에서 로비를 사용하도록
		SessionSettings.Set(SERVER_NAME_SETTING_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		// 세션이 아직 만들어지지 않았다 과정중
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
 }
