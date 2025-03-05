// Fill out your copyright notice in the Description page of Project Settings.


#include "ModeSelectionWidget.h"
#include "MenuWidgets/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SimpleDoorGameInstance.h"


void UModeSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UModeSelectionWidget::HostButtonClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UModeSelectionWidget::JoinButtonClicked);
	}
}

void UModeSelectionWidget::SetParentMenu(UMainMenuWidget* InParentMenu)
{
	ParentMenu = InParentMenu;
}

void UModeSelectionWidget::HostButtonClicked()
{
	USimpleDoorGameInstance* GameInstance = Cast<USimpleDoorGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (HostButton)
		{
			HostButton->SetIsEnabled(false);
			GameInstance->HostGame();
		}
	}
}

void UModeSelectionWidget::JoinButtonClicked()
{
	if (ParentMenu)
	{
		ParentMenu->ShowJoinGameWidget();
	}
}



