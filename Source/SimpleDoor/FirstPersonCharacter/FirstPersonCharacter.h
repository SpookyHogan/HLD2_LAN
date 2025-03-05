// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BasicCharacter.h"
#include "Logging/LogMacros.h"
#include "FirstPersonCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFirstPersonCharacter : public ABasicCharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
public:
	AFirstPersonCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void Server_Interact();

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	int Health;

public:

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AActor> WeaponBP;
		
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

public:
	/** Called for movement input */
	void Move(const FInputActionValue& Value) override;

	/** Called for looking input */
	void Look(const FInputActionValue& Value) override;

	/** Called for interact input */
	void Interact() override;

	/** Called for fire input */
	void Fire() override;

public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
protected:
	class UTP_WeaponComponent* WeaponComponent;
};

