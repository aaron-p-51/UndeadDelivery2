// Fill out your copyright notice in the Description page of Project Settings.


#include "UDLanSetupInstanceSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

void UUDLanSetupInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (!IConsoleManager::Get().IsNameRegistered(TEXT("UDHostTest")))
	{
		IConsoleManager::Get().RegisterConsoleCommand(TEXT("UDHostTest"), TEXT("Host testing session"), FConsoleCommandDelegate::CreateUObject(this, &UUDLanSetupInstanceSubsystem::UDHostTest));
	}

	if (!IConsoleManager::Get().IsNameRegistered(TEXT("UDHostGame")))
	{
		IConsoleManager::Get().RegisterConsoleCommand(TEXT("UDHostGame"), TEXT("Host testing session"), FConsoleCommandDelegate::CreateUObject(this, &UUDLanSetupInstanceSubsystem::UDHostGame));
	}

	if (!IConsoleManager::Get().IsNameRegistered(TEXT("UDJoin")))
	{
		IConsoleManager::Get().RegisterConsoleCommand(TEXT("UDJoin"), TEXT("Host testing session"), FConsoleCommandWithArgsDelegate::CreateUObject(this, &UUDLanSetupInstanceSubsystem::Join));
	}

	if (!IConsoleManager::Get().IsNameRegistered(TEXT("UDQuit")))
	{
		IConsoleManager::Get().RegisterConsoleCommand(TEXT("UDQuit"), TEXT("Quit testing session"), FConsoleCommandDelegate::CreateUObject(this, &UUDLanSetupInstanceSubsystem::Quit));
	}
}


void UUDLanSetupInstanceSubsystem::UDHostTest()
{
	UEngine* Engine = GetGameInstance()->GetEngine();
	if (Engine)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting"));
			World->ServerTravel("/Game/UndeadDelivery2/Maps/Map_MultiplayerTesting?listen");
		}
	}
}

void UUDLanSetupInstanceSubsystem::UDHostGame()
{
	UEngine* Engine = GetGameInstance()->GetEngine();
	if (Engine)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting"));
			World->ServerTravel("/Game/UndeadDelivery2/Maps/Map_PlayerCarTesting?listen");
		}
	}
}

void UUDLanSetupInstanceSubsystem::Join(const TArray<FString>& IpAddress)
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
				PlayerController->ClientTravel(ip, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UUDLanSetupInstanceSubsystem::Quit()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Map_Menu"));
}


