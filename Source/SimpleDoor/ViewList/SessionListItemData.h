// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SessionListItemData.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEDOOR_API USessionListItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString SessionName;
};
