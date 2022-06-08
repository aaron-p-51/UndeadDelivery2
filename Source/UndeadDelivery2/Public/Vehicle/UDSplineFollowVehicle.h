// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDSplineFollowVehicle.generated.h"

class USplineComponent;

UCLASS()
class UNDEADDELIVERY2_API AUDSplineFollowVehicle : public AActor
{
	GENERATED_BODY()
	

/**
 * Members
 */
public:

	/** Desired constant speed of vehicle in m/s */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float Speed;

private:

	float TotalPathDistance;
	float CurrentDistanceTraveled;

	UPROPERTY(ReplicatedUsing=OnRep_FollowSpline)
	USplineComponent* FollowSpline;

	float SpeedCentemetersPerSecond;

 /**
  * Methods
  */
public:	
	// Sets default values for this actor's properties
	AUDSplineFollowVehicle();

	void SetFollowSpline(USplineComponent* Spline);

	FORCEINLINE void SetSpeed(float Value) { Speed = Value; }

private:

	void CacheVariables();

	void MoveAlongSpline(float DeltaTime);

	UFUNCTION()
	void OnRep_FollowSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
