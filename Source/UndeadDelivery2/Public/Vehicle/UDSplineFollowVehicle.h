// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDSplineFollowVehicle.generated.h"

class USplineComponent;
class UBoxComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* FollowSpline;
};

USTRUCT()
struct FCollisionMove
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	float Time;
};


UCLASS()
class UNDEADDELIVERY2_API AUDSplineFollowVehicle : public AActor
{
	GENERATED_BODY()
	

/**
 * Members
 */
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category="LAN Testing")
	bool bSyncServerSpawnTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bCanMove;


private:

	float TotalPathDistance;
	float CurrentDistanceTraveled;
	float SpeedCentemetersPerSecond;

	bool bCollisionWithVehicleRegistered;

	TArray<FCollisionMove> UnacknowledgedCollisionMoves;

	UPROPERTY(ReplicatedUsing=OnRep_ServerStateCollisionMove);
	FCollisionMove ServerStateCollisionMove;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_SpawnData)
	FSpawnData SpawnData;


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

	UFUNCTION(BlueprintPure)
	float GetSpawnDataSpeed() { return SpawnData.Speed; }

private:

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;

	void SetupToFollowSpline();

	void MoveAlongSpline(float DeltaTime);

	FCollisionMove GetCollisionMove() const;

	void ClientTick(float DeltaTime);

	//UFUNCTION()
	//void OnRep_FollowSpline();

	UFUNCTION()
	void OnRep_SpawnData();

	UFUNCTION()
	void MeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnRep_ServerStateCollisionMove();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
