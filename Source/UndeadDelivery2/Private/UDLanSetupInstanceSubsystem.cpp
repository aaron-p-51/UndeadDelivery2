// Fill out your copyright notice in the Description page of Project Settings.


#include "UDLanSetupInstanceSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"

void UUDLanSetupInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("HostUD"), TEXT("Host testing session"), FConsoleCommandDelegate::CreateUObject(this, &UUDLanSetupInstanceSubsystem::HostUD));
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("JoinUD"), TEXT("Host testing session"), FConsoleCommandWithArgsDelegate::CreateUObject(this, &UUDLanSetupInstanceSubsystem::JoinUD));
}


void UUDLanSetupInstanceSubsystem::HostUD()
{
	UEngine* Engine = GetGameInstance()->GetEngine();
	if (Engine)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting"));
			//World->ServerTravel()
		}
	}
}

void UUDLanSetupInstanceSubsystem::JoinUD(const TArray<FString>& IpAddress)
{
	UEngine* Engine = GetGameInstance()->GetEngine();
	if (Engine)
	{
		if (IpAddress.Num() == 0)
		{
			Engine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("HostUD command required Ip address arg %s")));
		}
		else if (IpAddress.Num() == 1)
		{
			FString ip = *IpAddress[0];

			auto PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *ip));
				//PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

