// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SimpleDoorGameInstance.generated.h"

UCLASS()
class SIMPLEDOOR_API USimpleDoorGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;

    void HostGame();
    void FindGames(class UJoinUserWidget* UserWidget);
    void JoinGame(int32 SessionIndex);
    void LeaveSession();

private:
    TWeakPtr<IOnlineSession> SessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful); 

    virtual void Shutdown() override;
    UJoinUserWidget* TextRenderWidget;
};
	