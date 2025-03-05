// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseInterface.h"
#include "Bomb.generated.h"

UCLASS()
class SIMPLEDOOR_API ABomb : public AActor, public IUseInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere)
		class URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* TextComponent;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ParticleFX1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle TimerHandle_Fuse;

	int FuseTime;

	void OnExplode();

	void SetColor(FLinearColor color);

	UFUNCTION()
	void CountDown();

	void PlayExplodeParticleEffect();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Use_Implementation() override; 
	//virtual void Use_Implementation(APlayerController* PC = nullptr) override;


};
