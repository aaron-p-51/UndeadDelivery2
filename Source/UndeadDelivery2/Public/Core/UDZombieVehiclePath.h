// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDZombieVehiclePath.generated.h"

class USplineComponent;

UCLASS()
class UNDEADDELIVERY2_API AUDZombieVehiclePath : public AActor
{
	GENERATED_BODY()
	
/**
 * Members
 */
protected:

	UPROPERTY(VisibleAnywhere)
	USplineComponent* VehiclePath;

public:

	UPROPERTY(EditAnywhere, Category="Configuration")
	bool bIsActive;

public:	
	// Sets default values for this actor's properties
	AUDZombieVehiclePath();

	FORCEINLINE USplineComponent* GetPath() const { return VehiclePath; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
