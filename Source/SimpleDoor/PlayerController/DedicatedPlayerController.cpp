// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BasicCharacter.h"


void ADedicatedPlayerController::OnPossess(APawn* PossesedPawn)
{
    Super::OnPossess(PossesedPawn);
}

void ADedicatedPlayerController::AcknowledgePossession(APawn* PossesedPawn)
{
    Super::AcknowledgePossession(PossesedPawn);
    PossessedPawn = Cast<ABasicCharacter>(PossesedPawn);
}

void ADedicatedPlayerController::OnUnPossess()
{
    Super::OnUnPossess();
}

void ADedicatedPlayerController::BeginPlay()
{
    Super::BeginPlay();
    //Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void ADedicatedPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADedicatedPlayerController::Move);
       
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADedicatedPlayerController::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADedicatedPlayerController::StopJumping);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADedicatedPlayerController::Look);

        // Interact
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ADedicatedPlayerController::Interact);

        // Fire
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ADedicatedPlayerController::Fire);
        
    }
}

void ADedicatedPlayerController::Move(const FInputActionValue& Value)
{
    if (PossessedPawn)
    {
        PossessedPawn->Move(Value);
    }
}

void ADedicatedPlayerController::Look(const FInputActionValue& Value)
{
    if (PossessedPawn)
    {
        PossessedPawn->Look(Value);
    }
}

void ADedicatedPlayerController::Jump(const FInputActionValue& Value)
{
    if (PossessedPawn)
    {
        PossessedPawn->Jump();
    }
}

void ADedicatedPlayerController::StopJumping(const FInputActionValue& Value)
{
    if (PossessedPawn)
    {
        PossessedPawn->StopJumping();
    }
}

void ADedicatedPlayerController::Interact()
{
    if (PossessedPawn)
    {
        PossessedPawn->Interact();
    }
}

void ADedicatedPlayerController::Fire()
{
    if (PossessedPawn)
    {
        PossessedPawn->Fire();
    }
}
