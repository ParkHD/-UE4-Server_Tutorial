// Fill out your copyright notice in the Description page of Project Settings.


#include "00_MenuSystem/ServerRow.h"

#include "00_MenuSystem/MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(class UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddUniqueDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}
