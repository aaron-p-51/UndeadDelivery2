// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UDLanSetupInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNDEADDELIVERY2_API UUDLanSetupInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

/**
 * Methods
 */

public:

	UFUNCTION()
	void Host();

	UFUNCTION()
	void Join(const TArray<FString>& IpAddress);

	UFUNCTION()
	void Quit();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
