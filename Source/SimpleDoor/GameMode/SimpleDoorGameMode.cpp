// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleDoorGameMode.h"
#include "BasicCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ASimpleDoorGameMode::ASimpleDoorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCharacter/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASimpleDoorGameMode::RespawnPlayer(AController* PlayerController)
{
    if (PlayerController)
    {
        FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), FVector(900.0f, 700.0f, 92.1f));
        if (PlayerStarts.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);

            SpawnTransform.SetLocation( PlayerStarts[RandomIndex]->GetActorLocation());
        }
        
        ABasicCharacter* NewCharacter = GetWorld()->SpawnActor<ABasicCharacter>(DefaultPawnClass, SpawnTransform);
        if (NewCharacter)
        {
            PlayerController->Possess(NewCharacter);
        }
    }
}

void ASimpleDoorGameMode::BeginPlay()
{
    Super::BeginPlay();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
}
