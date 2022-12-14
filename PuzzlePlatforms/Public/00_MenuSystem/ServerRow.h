// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HostUser;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ConnectionPraction;
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;
private:
	UFUNCTION()
		void OnClicked();

private:
	UPROPERTY()
		class UMainMenu* Parent;
	uint32 Index;
public:
	void Setup(class UMainMenu* InParent, uint32 InIndex);

public:
	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;
};
