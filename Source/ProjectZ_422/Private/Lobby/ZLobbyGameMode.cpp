// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyGameMode.h"
#include "ZLobbyPlayerController.h"
#include "ZLobbyGameState.h"
#include "ZPlayerState.h"
#include "ZGameInstance.h"
#include "..\..\Public\Lobby\ZLobbyGameMode.h"
#include "Engine/World.h"

void AZLobbyGameMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		ZLOG(Error, TEXT("Host : %s"), *MyGameInstance->GetWebConnector().GetIP());

		FString URL = *MyGameInstance->GetWebConnector().GetWebURL();
		URL.Append(TEXT("/create_game"));

		FString PostParam = FString::Printf(TEXT("ip=%s"), *MyGameInstance->GetWebConnector().GetIP());

		MyGameInstance->GetWebConnector().HttpPost(URL, PostParam);

	}
	

}

void AZLobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++ConnectNumber;
	
	auto MyGameState = GetGameState<AZLobbyGameState>();
	if (MyGameState)
	{
		MyGameState->SetConnectNumber(ConnectNumber);
	}

}

void AZLobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	if (ConnectNumber > 0)
	{
		--ConnectNumber;
	}
	
	auto MyGameState = GetGameState<AZLobbyGameState>();
	if (MyGameState)
	{
		MyGameState->SetConnectNumber(ConnectNumber);
	}
	
	auto MyPS = Exiting->GetPlayerState<AZPlayerState>();
	if (nullptr == MyPS)
	{
		ZLOG(Error, TEXT("Invalid player state.."));
		return;
	}

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		auto LobbyPC = Cast<AZLobbyPlayerController>(*Iter);
		if (LobbyPC)
		{
			LobbyPC->ClientUpdateJoinPlayer(MyPS->GetPlayerName(), true);
		}
	}
	
	

}

void AZLobbyGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason != EEndPlayReason::LevelTransition)
	{
		auto MyGameInstance = GetGameInstance<UZGameInstance>();
		if (MyGameInstance)
		{
			ZLOG(Error, TEXT("Host : %s"), *MyGameInstance->GetWebConnector().GetIP());

			FString URL = *MyGameInstance->GetWebConnector().GetWebURL();
			URL.Append(TEXT("/delete_game"));

			FString PostParam = FString::Printf(TEXT("ip=%s"), *MyGameInstance->GetWebConnector().GetIP());

			MyGameInstance->GetWebConnector().HttpPost(URL, PostParam);

		}
	}

	Super::EndPlay(EndPlayReason);
}

void AZLobbyGameMode::StartGame()
{
	ZLOG_S(Error);
	if (!GetWorld()->ServerTravel(TEXT("Stage1")))
	{
		ZLOG(Error, TEXT("Server travel fail."));
		return;
	}

}

int32 AZLobbyGameMode::GetConnectNumber() const
{
	return ConnectNumber;
}
