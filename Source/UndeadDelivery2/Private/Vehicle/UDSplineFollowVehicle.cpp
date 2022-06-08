// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/UDSplineFollowVehicle.h"

// Engine Includes
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"

// Game Includes

// Sets default values
AUDSplineFollowVehicle::AUDSplineFollowVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TotalPathDistance = 0.f;
	CurrentDistanceTraveled = 0.f;

	SetReplicates(true);
	SetReplicateMovement(false);
}


// Called when the game starts or when spawned
void AUDSplineFollowVehicle::BeginPlay()
{
	Super::BeginPlay();
	
	CacheVariables();
}


void AUDSplineFollowVehicle::CacheVariables()
{
	// Convert Speed m/s to cm/s
	SpeedCentemetersPerSecond = Speed * 100.f;

	if (FollowSpline)
	{
		TotalPathDistance = FollowSpline->GetSplineLength();
	}
}


// Called every frame
void AUDSplineFollowVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowSpline)
	{
		MoveAlongSpline(DeltaTime);
		if (CurrentDistanceTraveled >= TotalPathDistance)
		{
			Destroy();
		}
	}
}


void AUDSplineFollowVehicle::MoveAlongSpline(float DeltaTime)
{
	CurrentDistanceTraveled = CurrentDistanceTraveled + (Speed * 100.f * DeltaTime); //SpeedCentemetersPerSecond * DeltaTime;
	
	FTransform TransformOnSpline = FollowSpline->GetTransformAtDistanceAlongSpline(CurrentDistanceTraveled, ESplineCoordinateSpace::World);
	SetActorTransform(TransformOnSpline);
}


void AUDSplineFollowVehicle::OnRep_FollowSpline()
{
	TotalPathDistance = FollowSpline->GetSplineLength();
}

void AUDSplineFollowVehicle::SetFollowSpline(USplineComponent* Spline)
{
	FollowSpline = Spline;

	if (FollowSpline)
	{
		TotalPathDistance = FollowSpline->GetSplineLength();
	}
}

void AUDSplineFollowVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUDSplineFollowVehicle, FollowSpline);
	DOREPLIFETIME(AUDSplineFollowVehicle, Speed);
}

