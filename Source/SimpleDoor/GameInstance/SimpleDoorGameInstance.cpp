// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleDoorGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <Online/OnlineSessionNames.h>
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "JoinUserWidget.h"


void USimpleDoorGameInstance::Init()
{
    Super::Init();

    IOnlineSessionPtr OnlineSubsystem = Online::GetSessionInterface(GetWorld());
    if (OnlineSubsystem)
    {
        SessionInterface = OnlineSubsystem;
    }
}

void USimpleDoorGameInstance::HostGame()
{
    if (TSharedPtr<IOnlineSession> Session = SessionInterface.Pin())
    {
        if (Session->GetNamedSession(NAME_GameSession))
        {
            UE_LOG(LogTemp, Warning, TEXT("Session already exists, destroying before creating a new one..."));
            Session->DestroySession(NAME_GameSession);
        }

        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = true;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.NumPublicConnections = 8;
        SessionSettings.Set(SETTING_MAPNAME, FString(TEXT("Week 9 Session")), EOnlineDataAdvertisementType::ViaOnlineService);
        Session->OnCreateSessionCompleteDelegates.AddUObject(this, &USimpleDoorGameInstance::OnCreateSessionComplete);
        Session->CreateSession(0, NAME_GameSession, SessionSettings);
    }
    
}

void USimpleDoorGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UGameplayStatics::OpenLevel(this, "ThirdPersonMap", true, "listen");
       // GetWorld()->ServerTravel("ThirdPersonMap?listen");
    }
}

void USimpleDoorGameInstance::FindGames(UJoinUserWidget* UserWidget)
{
    if (TSharedPtr<IOnlineSession> Session = SessionInterface.Pin())
    {
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 10;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        Session->OnFindSessionsCompleteDelegates.RemoveAll(this);
        Session->OnFindSessionsCompleteDelegates.AddUObject(this, &USimpleDoorGameInstance::OnFindSessionsComplete);
        Session->FindSessions(0, SessionSearch.ToSharedRef());
        TextRenderWidget = UserWidget;
    }
}

void USimpleDoorGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
    {
        if (TextRenderWidget)
        {
            for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
            {
                FString MapName = SearchResult.Session.SessionSettings.Settings.Contains("MAPNAME")
                    ? *SearchResult.Session.SessionSettings.Settings.Find("MAPNAME")->Data.ToString()
                    : TEXT("Unknown Map");
                int32 MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
                int32 CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
                TextRenderWidget->OnFindSessionsComplete(MapName + " " + FString::FromInt(CurrentPlayers) +" out of " + FString::FromInt(MaxPlayers));
            }
        }
    }
    else
    {
        if (TextRenderWidget)
        {
            TextRenderWidget->EnableRefresh();
        }
    }
}

void USimpleDoorGameInstance::LeaveSession()
{
    if (TSharedPtr<IOnlineSession> Session = SessionInterface.Pin())
    {
        Session->OnDestroySessionCompleteDelegates.AddUObject(this, &USimpleDoorGameInstance::OnDestroySessionComplete);
        Session->DestroySession(NAME_GameSession);
    }
}

void USimpleDoorGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session destroyed successfully. Returning to MainMenu."));

        APlayerController* PlayerController = GetFirstLocalPlayerController();
        if (PlayerController)
        {
            PlayerController->ClientTravel("/Game/ThirdPerson/Maps/ModeSelectionScreen", ETravelType::TRAVEL_Absolute);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session."));
    }
}

void USimpleDoorGameInstance::Shutdown()
{
    Super::Shutdown();
    LeaveSession();
}

void USimpleDoorGameInstance::JoinGame(int32 SessionIndex)
{
    if (!SessionSearch.IsValid() || SessionSearch->SearchResults.Num() <= SessionIndex)
        return;
    if (TSharedPtr<IOnlineSession> Session = SessionInterface.Pin())
    {
        Session->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[SessionIndex]);
        ClientTravelToSession(0, NAME_GameSession);
    }
}