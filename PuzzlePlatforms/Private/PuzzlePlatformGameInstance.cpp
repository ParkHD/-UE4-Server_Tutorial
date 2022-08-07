// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// 생성자는 에디터 재실행시에도 호출된다.
	UE_LOG(LogTemp, Log, TEXT("Constructor"));
}

void UPuzzlePlatformGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Log, TEXT("Init"));

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
