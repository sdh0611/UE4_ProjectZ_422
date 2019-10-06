// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyGameMode.h"
#include "ZLobbyPlayerController.h"
#include "ZLobbyGameState.h"
#include "..\..\Public\Lobby\ZLobbyGameMode.h"
#include "Engine/World.h"

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

}

void AZLobbyGameMode::StartGame()
{
	ZLOG_S(Error);
	if (!GetWorld()->ServerTravel(TEXT("Stage1")))
	{
		ZLOG(Error, TEXT("Server travel fail."));
	}

}

int32 AZLobbyGameMode::GetConnectNumber() const
{
	return ConnectNumber;
}
