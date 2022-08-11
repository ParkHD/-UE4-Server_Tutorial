// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayer;
	UE_LOG(LogTemp, Warning, TEXT("Lonin"));

	// GameMode�� �������� �ϳ��� �����ȴ�.
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