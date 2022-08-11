// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayer;
	UE_LOG(LogTemp, Warning, TEXT("Lonin"));

	// GameMode는 서버에서 하나만 생성된다.
	if(NumberOfPlayer >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 players!"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NumberOfPlayer--;

}
