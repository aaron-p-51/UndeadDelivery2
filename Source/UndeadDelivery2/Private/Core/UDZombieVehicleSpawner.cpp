// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UDZombieVehicleSpawner.h"

// Engine Includes
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Containers/Set.h"

// Game Includes
#include "Core/UDZombieVehiclePath.h"
#include "Vehicle/UDSplineFollowVehicle.h"



// Sets default values
AUDZombieVehicleSpawner::AUDZombieVehicleSpawner()
{
	//SetReplicates(true);

}

// Called when the game starts or when spawned
void AUDZombieVehicleSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		GetAllZombieVehiclePaths();
		GetWorld()->GetTimerManager().SetTimer(SpawnVehicleTimerHandle, this, &AUDZombieVehicleSpawner::SpawnZombieVehicle, GetSpawnVehicleDelay(), false);
	}
	
}


void AUDZombieVehicleSpawner::SpawnZombieVehicle()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnVehicleTimerHandle);


	auto PossiblePaths = GetPossiblePaths();
	if (PossiblePaths.Num() <= 0)
	{
		UsedZombieVehiclePaths.Empty();
		GetWorld()->GetTimerManager().SetTimer(SpawnVehicleTimerHandle, this, &AUDZombieVehicleSpawner::SpawnZombieVehicle, GetSpawnVehicleDelay(), false);
		return;
	}

	auto SpawnPath = PossiblePaths[UKismetMathLibrary::RandomIntegerInRange(0, PossiblePaths.Num() - 1)];

	// Spawn
	FTransform VehcileSpawnTransform(SpawnPath->GetActorTransform());
	auto ZombieSpawnVehicle = Cast<AUDSplineFollowVehicle>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ZombieVehicleClass, VehcileSpawnTransform));
	if (ZombieSpawnVehicle)
	{
		ZombieSpawnVehicle->Speed = UKismetMathLibrary::RandomFloatInRange(MinVehicleSpeed, MaxSpawnVehicleDelay);
		ZombieSpawnVehicle->SetFollowSpline(SpawnPath->GetPath());
		UGameplayStatics::FinishSpawningActor(ZombieSpawnVehicle, VehcileSpawnTransform);
	}

	UsedZombieVehiclePaths.Add(SpawnPath);
	GetWorld()->GetTimerManager().SetTimer(SpawnVehicleTimerHandle, this, &AUDZombieVehicleSpawner::SpawnZombieVehicle, GetSpawnVehicleDelay(), false);
}


void AUDZombieVehicleSpawner::GetAllZombieVehiclePaths()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ZombieVehiclePathClass, FoundActors);
	for (const auto& FoundActor : FoundActors)
	{
		auto ZombieVehiclePath = Cast<AUDZombieVehiclePath>(FoundActor);
		if (ZombieVehiclePath)
		{
			ZombieVehiclePaths.Add(ZombieVehiclePath);
		}
	}
}


float AUDZombieVehicleSpawner::GetSpawnVehicleDelay() const
{
	return UKismetMathLibrary::RandomFloatInRange(MinSpawnVehcileDelay, MaxSpawnVehicleDelay);
}


TArray<AUDZombieVehiclePath*> AUDZombieVehicleSpawner::GetPossiblePaths() const
{
	TArray<AUDZombieVehiclePath*> PossiblePaths;
	for (const auto& Path : ZombieVehiclePaths)
	{
		if (Path->bIsActive && !UsedZombieVehiclePaths.Contains(Path))
		{
			PossiblePaths.Add(Path);
		}
	}

	return PossiblePaths;
}

