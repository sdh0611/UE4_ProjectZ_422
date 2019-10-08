// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyPlayerController.h"
#include "..\..\Public\Lobby\ZLobbyPlayerController.h"
#include "Lobby/ZLobbyWidget.h"
#include "Lobby/ZLobbyGameMode.h"
#include "Lobby/ZLobbyGameState.h"
#include "ZPlayerState.h"
#include "ZCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


void AZLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (UserHUDClass)
		{
			UserHUD = CreateWidget<UZLobbyWidget>(this, UserHUDClass);
			if (UserHUD)
			{
				UserHUD->AddToViewport();
				bShowMouseCursor = true;
				SetInputMode(FInputModeGameAndUI());
				if (HasAuthority())
				{
					UserHUD->PlayFadeInStartButtonAnim();
				}

				auto MyGameState = GetWorld()->GetGameState<AZLobbyGameState>();
				if (MyGameState)
				{
					UpdateConnectNumber(MyGameState->GetConnectNumber());
				}
			}
		}
	}


}

void AZLobbyPlayerController::UpdateConnectNumber(int32 NewNumber)
{
	if (IsLocalPlayerController())
	{
		if (UserHUD)
		{
			UserHUD->UpdateConnectNumber(NewNumber);
		}
	}

}

void AZLobbyPlayerController::UpdatePlayerName(const FString & PlayerName, bool bErase)
{
	if (IsLocalPlayerController())
	{
		if (UserHUD)
		{
			if (bErase)
			{
				UserHUD->DeletePlayerName(PlayerName);
			}
			else
			{
				UserHUD->UpdatePlayerName(PlayerName);
			}

		}
	}

}

bool AZLobbyPlayerController::ServerReceiveChat_Validate(const FString & PlayerName, const FString & RecvChat)
{
	return true;
}

void AZLobbyPlayerController::ServerReceiveChat_Implementation(const FString & PlayerName, const FString & RecvChat)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		auto PC = Cast<AZLobbyPlayerController>(*Iter);
		if (PC)
		{
			PC->ClientReceiveChat(PlayerName, RecvChat);
		}
	}


}

bool AZLobbyPlayerController::ClientReceiveChat_Validate(const FString & PlayerName, const FString& RecvChat)
{
	return true;
}

void AZLobbyPlayerController::ClientReceiveChat_Implementation(const FString & PlayerName, const FString& RecvChat)
{
	if (nullptr == UserHUD)
	{
		ZLOG(Error, TEXT("Lobby UserHUD null."));
		return;
	}

	UserHUD->UpdateChatBox(PlayerName, RecvChat);

}

bool AZLobbyPlayerController::ClientUpdateJoinPlayer_Validate(const FString & JoinPlayer, bool bErase)
{
	return true;
}

void AZLobbyPlayerController::ClientUpdateJoinPlayer_Implementation(const FString & JoinPlayer, bool bErase)
{
	UpdatePlayerName(JoinPlayer, bErase);
}

bool AZLobbyPlayerController::ServerReceiveUpdateJoinPlayer_Validate()
{
	return true;
}

void AZLobbyPlayerController::ServerReceiveUpdateJoinPlayer_Implementation()
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		if (Iter->Get() != this)
		{
			auto MyPS = Iter->Get()->GetPlayerState<AZPlayerState>();
			if (MyPS)
			{
				ClientUpdateJoinPlayer(MyPS->GetPlayerName(), false);
			}
		}
	}


}

void AZLobbyPlayerController::OnReceiveUserName(const FString & UserName)
{
	Super::OnReceiveUserName(UserName);

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ZLOG_S(Error);
		auto LobbyPC = Cast<AZLobbyPlayerController>(*Iter);
		if (LobbyPC)
		{
			LobbyPC->ClientUpdateJoinPlayer(UserName, false);
		}
	}

}


