// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyGameMode.h"
#include "..\..\Public\Lobby\ZLobbyGameMode.h"
#include "Engine/World.h"

void AZLobbyGameMode::StartGame()
{
	ZLOG_S(Error);
	if (!GetWorld()->ServerTravel(TEXT("Stage1")))
	{
		ZLOG(Error, TEXT("Server travel fail."));
	}

}
