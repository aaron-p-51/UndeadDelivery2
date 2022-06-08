// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDZombieVehicleSpawner.generated.h"

class AUDZombieVehiclePath;
class AUDSplineFollowVehicle;



UCLASS()
class UNDEADDELIVERY2_API AUDZombieVehicleSpawner : public AActor
{
	GENERATED_BODY()

/**
 * Members
 */

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AUDZombieVehiclePath> ZombieVehiclePathClass;

	UPROPERTY()
	TArray<AUDZombieVehiclePath*> ZombieVehiclePaths;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AUDSplineFollowVehicle> ZombieVehicleClass;





	UPROPERTY(EditAnywhere, Category="Configuration")
	float MinSpawnVehcileDelay;

	UPROPERTY(EditAnywhere, Category="Configuration")
	float MaxSpawnVehicleDelay;

	UPROPERTY(EditAnywhere, Category="Configuration")
	float MinVehicleSpeed;

	UPROPERTY(EditAnywhere, Category="Configuration")
	float MaxVehicleSpeed;





private:

	FTimerHandle SpawnVehicleTimerHandle;

	TSet<AUDZombieVehiclePath*> UsedZombieVehiclePaths;

 /**
  * Methods
  */
	
public:	
	// Sets default values for this actor's properties
	AUDZombieVehicleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	void GetAllZombieVehiclePaths();

	void SpawnZombieVehicle();

	float GetSpawnVehicleDelay() const;

	TArray<AUDZombieVehiclePath*> GetPossiblePaths() const;

};
