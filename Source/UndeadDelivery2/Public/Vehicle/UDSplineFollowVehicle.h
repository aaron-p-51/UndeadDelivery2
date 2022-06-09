// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDSplineFollowVehicle.generated.h"

class USplineComponent;

USTRUCT()
struct FSpawnData
{
	GENERATED_BODY()

	UPROPERTY()
	float Speed;

	UPROPERTY()
	float SpawnTime;

	UPROPERTY()
	USplineComponent* FollowSpline;
};

UCLASS()
class UNDEADDELIVERY2_API AUDSplineFollowVehicle : public AActor
{
	GENERATED_BODY()
	

/**
 * Members
 */
public:

	///** Desired constant speed of vehicle in m/s */
	//UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	//float Speed;

	//UPROPERTY(Replicated)
	//float SpawnTime;

	UPROPERTY(EditAnywhere, Category="LAN Testing")
	bool bSyncServerSpawnTime;

private:

	float TotalPathDistance;
	float CurrentDistanceTraveled;

	UPROPERTY(ReplicatedUsing = OnRep_SpawnData)
	FSpawnData SpawnData;

	//UPROPERTY(ReplicatedUsing=OnRep_FollowSpline)
	//USplineComponent* FollowSpline;

	//float SpeedCentemetersPerSecond;

 /**
  * Methods
  */
public:	
	// Sets default values for this actor's properties
	AUDSplineFollowVehicle();

	//void SetFollowSpline(USplineComponent* Spline);

	UFUNCTION()
	void SetSpawnData(FSpawnData Value);

	/*FORCEINLINE void SetSpeed(float Value) { Speed = Value; }*/

private:

	//void CacheVariables();

	void MoveAlongSpline(float DeltaTime);

	//UFUNCTION()
	//void OnRep_FollowSpline();

	UFUNCTION()
	void OnRep_SpawnData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
