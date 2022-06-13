// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/UDSplineFollowVehicle.h"

// Engine Includes
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Game Includes
#include "Pawn/UDPlayerVehiclePawn.h"


// Sets default values
AUDSplineFollowVehicle::AUDSplineFollowVehicle()
{
	/*RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());*/
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	

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

	if (MeshComponent)
	{
		MeshComponent->OnComponentHit.AddDynamic(this, &AUDSplineFollowVehicle::MeshComponentHit);
	}
	
	SetupToFollowSpline();
	if (SpawnData.FollowSpline)
	{
		TotalPathDistance = SpawnData.FollowSpline->GetSplineLength();
	}
}


void AUDSplineFollowVehicle::SetupToFollowSpline()
{
	// Convert Speed m/s to cm/s
	SpeedCentemetersPerSecond = SpawnData.Speed * 100.f;

	if (SpawnData.FollowSpline)
	{
		TotalPathDistance = SpawnData.FollowSpline->GetSplineLength();
	}
}


// Called every frame
void AUDSplineFollowVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCollisionWithVehicleRegistered)
	{
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
	else
	{
		if (GetLocalRole() == ENetRole::ROLE_Authority)
		{
			ServerStateCollisionMove = GetCollisionMove();
		}
		else
		{
			ClientTick(DeltaTime);
		}
	}
}

void AUDSplineFollowVehicle::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdates > KINDA_SMALL_NUMBER)
	{
		float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;

		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = ServerStateCollisionMove.Transform.GetLocation();
		FVector LerpLocation = FMath::Lerp<FVector>(StartLocation, TargetLocation, LerpRatio);

		FQuat StartRotation = GetActorRotation().Quaternion();
		FQuat TargetRotation = ServerStateCollisionMove.Transform.GetRotation();
		FQuat LerpRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);

		SetActorLocationAndRotation(LerpLocation, LerpRotation);
	}
}

void AUDSplineFollowVehicle::MoveAlongSpline(float DeltaTime)
{
	CurrentDistanceTraveled = CurrentDistanceTraveled + (SpawnData.Speed * 100.f * DeltaTime); //SpeedCentemetersPerSecond * DeltaTime;
	FTransform TransformOnSpline = SpawnData.FollowSpline->GetTransformAtDistanceAlongSpline(CurrentDistanceTraveled, ESplineCoordinateSpace::World);

	SetActorTransform(TransformOnSpline, true);
}



FCollisionMove AUDSplineFollowVehicle::GetCollisionMove() const
{
	FCollisionMove CollisionMove;
	CollisionMove.Transform = GetActorTransform();

	auto GameState = GetWorld()->GetGameState();
	if (GameState)
	{
		CollisionMove.Time = GameState->GetServerWorldTimeSeconds();
	}
	else
	{
		CollisionMove.Time = GetWorld()->GetTimeSeconds();
	}

	return CollisionMove;
}



void AUDSplineFollowVehicle::OnRep_SpawnData()
{
	SetupToFollowSpline();

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


void AUDSplineFollowVehicle::MeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bCollisionWithVehicleRegistered) return;
	

	/** Create interface do determine proper collision actor */
	auto OtherSplineFollowVehicle = Cast<AUDSplineFollowVehicle>(OtherActor);
	auto OtherPlayerVehiclePawn = Cast<AUDPlayerVehiclePawn>(OtherActor);
	if (OtherSplineFollowVehicle || OtherPlayerVehiclePawn)
	{
		if (MeshComponent)
		{
			if (GetLocalRole() == ENetRole::ROLE_Authority)
			{
				MeshComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
				MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				MeshComponent->SetSimulatePhysics(true);
				MeshComponent->AddForce(Hit.ImpactNormal * 5.f, NAME_None, true);
				MeshComponent->SetEnableGravity(true);
			}
			bCollisionWithVehicleRegistered = true;
			//SetLifeSpan(5.f);
		}
	}
}


void AUDSplineFollowVehicle::OnRep_ServerStateCollisionMove()
{
	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.f;
}

void AUDSplineFollowVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AUDSplineFollowVehicle, FollowSpline);
	//DOREPLIFETIME(AUDSplineFollowVehicle, Speed);
	DOREPLIFETIME(AUDSplineFollowVehicle, SpawnData);
	DOREPLIFETIME(AUDSplineFollowVehicle, bCanMove);
	DOREPLIFETIME(AUDSplineFollowVehicle, ServerStateCollisionMove);
}

