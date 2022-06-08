// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UDZombieVehiclePath.h"

// Engine Includes
#include "Components/SplineComponent.h"

// Sets default values
AUDZombieVehiclePath::AUDZombieVehiclePath()
{
	VehiclePath = CreateDefaultSubobject<USplineComponent>(TEXT("Vehicle Path"));
	SetRootComponent(VehiclePath);


}

// Called when the game starts or when spawned
void AUDZombieVehiclePath::BeginPlay()
{
	Super::BeginPlay();

	
	
}



