// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "UseInterface.h"
#include "SimpleDoorGameMode.h"
#include "TP_WeaponComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFirstPersonCharacter::AFirstPersonCharacter():
	Health(100)
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	RootComponent = GetCapsuleComponent();
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), FName("head"));
	FirstPersonCameraComponent->SetRelativeLocation(FVector(1.75f, 9.85f, 0)); // Position the camera
	FirstPersonCameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 80.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	OnTakeAnyDamage.AddDynamic(this, &AFirstPersonCharacter::TakeAnyDamage);

	SetReplicates(true);
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AActor* Weapon = GetWorld()->SpawnActor<AActor>(WeaponBP);
		if (Weapon)
		{
			WeaponComponent = Weapon->FindComponentByClass<UTP_WeaponComponent>();
			if (WeaponComponent)
			{
				WeaponComponent->AttachWeapon(this);
				SetHasRifle(true);
			}
		}
	}
}

void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirstPersonCharacter::Interact()
{
	Server_Interact();
}

void AFirstPersonCharacter::Fire()
{
	if (WeaponComponent)
	{
		WeaponComponent->Fire();
	}
}

void AFirstPersonCharacter::Server_Interact_Implementation()
{
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + GetControlRotation().Vector() * 500.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams(), QueryParams))
	{
		if (AActor* Actor = HitResult.GetActor())
		{
			// check if server and call server rpc
			if (HasAuthority())
			{
				if (Actor->Implements<UUseInterface>())
				{
					IUseInterface::Execute_Use(Actor);
				}
			}
		}
	}
}

void AFirstPersonCharacter::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Damage Received - " + FString::FromInt(Damage) + "---" + DamageCauser->GetName());
	Health -= Damage;
	if (Health <= 0)
	{
		DisableInput(nullptr);
		ASimpleDoorGameMode* GM = Cast<ASimpleDoorGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			FTimerHandle TimerHandle;
			AController* ThisController = GetController();
			
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GM, ThisController]()
			{
				GM->RespawnPlayer(ThisController);
			}, 5.0f, false);

			if (WeaponComponent)
			{
				FString name = WeaponComponent->GetOwner()->GetName();
				WeaponComponent->GetOwner()->Destroy();
			}
			Destroy();
		}
		
	}
}

void AFirstPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFirstPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFirstPersonCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AFirstPersonCharacter::GetHasRifle()
{
	return bHasRifle;
}