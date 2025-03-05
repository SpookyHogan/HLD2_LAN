// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseInterface.h"
#include "Door.generated.h"

class UAnimationAsset;

UCLASS()
class SIMPLEDOOR_API ADoor : public AActor, public IUseInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	UPROPERTY(EditAnywhere, Category = "Tutorial")
	class USkeletalMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	UAnimationAsset* OpenAnimation;
	UPROPERTY(EditAnywhere, Category = "Tutorial")
	UAnimationAsset* CloseAnimation;

	UPROPERTY(Replicated)
	bool bDoorOpen;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DoorToggled();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Use_Implementation() override; 
	//virtual void Use_Implementation(APlayerController* PC) override;
};
