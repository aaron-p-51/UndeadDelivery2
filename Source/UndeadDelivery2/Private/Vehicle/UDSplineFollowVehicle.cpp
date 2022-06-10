// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/UDSplineFollowVehicle.h"

// Engine Includes
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Game Includes

// Sets default values
AUDSplineFollowVehicle::AUDSplineFollowVehicle()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TotalPathDistance = 0.f;
	CurrentDistanceTraveled = 0.f;

	bCanMove = true;

	bReplicates = true;
	SetReplicateMovement(false);
}


void AUDSplineFollowVehicle::SetSpawnData(FSpawnData Value)
{
	SpawnData = Value;
}

// Called when the game starts or when spawned
void AUDSplineFollowVehicle::BeginPlay()
{
	Super::BeginPlay();
	
	//CacheVariables();
	if (SpawnData.FollowSpline)
	{
		TotalPathDistance = SpawnData.FollowSpline->GetSplineLength();
	}
}


//void AUDSplineFollowVehicle::CacheVariables()
//{
//	// Convert Speed m/s to cm/s
//	SpeedCentemetersPerSecond = Speed * 100.f;
//
//	if (FollowSpline)
//	{
//		TotalPathDistance = FollowSpline->GetSplineLength();
//	}
//}


// Called every frame
void AUDSplineFollowVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnData.FollowSpline && TotalPathDistance > 0.f && bCanMove)
	{
		MoveAlongSpline(DeltaTime);
		if (CurrentDistanceTraveled >= TotalPathDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroy"));
			Destroy();
		}
	}
}


void AUDSplineFollowVehicle::MoveAlongSpline(float DeltaTime)
{
	CurrentDistanceTraveled = CurrentDistanceTraveled + (SpawnData.Speed * 100.f * DeltaTime); //SpeedCentemetersPerSecond * DeltaTime;
	FTransform TransformOnSpline = SpawnData.FollowSpline->GetTransformAtDistanceAlongSpline(CurrentDistanceTraveled, ESplineCoordinateSpace::World);

	FHitResult* HitResult = nullptr;
	SetActorLocation(TransformOnSpline.GetLocation(), true, HitResult);
	if (HitResult)
	{
		OnSweepHit(*HitResult);
	}

	//FHitResult* HitResult = nullptr;
	//SetActorTransform(TransformOnSpline, true, HitResult);
	//if (HitResult)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Sweep Hit"));
	//	OnSweepHit(*HitResult);
	//}
}


//void AUDSplineFollowVehicle::OnRep_FollowSpline()
//{
//	TotalPathDistance = FollowSpline->GetSplineLength();
//}



//void AUDSplineFollowVehicle::SetFollowSpline(USplineComponent* Spline)
//{
//	FollowSpline = Spline;
//
//	if (FollowSpline)
//	{
//		TotalPathDistance = FollowSpline->GetSplineLength();
//	}
//}

void AUDSplineFollowVehicle::OnRep_SpawnData()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep"));
	if (SpawnData.FollowSpline)
	{
		TotalPathDistance = SpawnData.FollowSpline->GetSplineLength();
	}

	if (bSyncServerSpawnTime)
	{
		auto GameStateBase = GetWorld()->GetGameState();
		if (GameStateBase)
		{
			float CurrentTime = GameStateBase->GetServerWorldTimeSeconds();
			float SpawnDelay = CurrentTime - SpawnData.SpawnTime;
			UE_LOG(LogTemp, Warning, TEXT("SyncServerSpawnTime: SpawnDelay: %f"), SpawnDelay);
			
			MoveAlongSpline(SpawnDelay);
		}
	}
}


void AUDSplineFollowVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AUDSplineFollowVehicle, FollowSpline);
	//DOREPLIFETIME(AUDSplineFollowVehicle, Speed);
	DOREPLIFETIME(AUDSplineFollowVehicle, SpawnData);
	DOREPLIFETIME(AUDSplineFollowVehicle, bCanMove);
}

