// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModeSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEDOOR_API UModeSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* HostButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* JoinButton = nullptr;

	void SetParentMenu(class UMainMenuWidget* InParentMenu);

protected:
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();

	UPROPERTY()
	class UMainMenuWidget* ParentMenu;
};
