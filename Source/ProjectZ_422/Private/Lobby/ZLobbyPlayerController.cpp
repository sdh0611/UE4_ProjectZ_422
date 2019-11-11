// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyPlayerController.h"
#include "..\..\Public\Lobby\ZLobbyPlayerController.h"
#include "Lobby/ZLobbyWidget.h"
#include "Lobby/ZLobbyGameMode.h"
#include "Lobby/ZLobbyGameState.h"
#include "ZPlayerState.h"
#include "ZCharacter.h"
#include "ZGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"

void AZLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		return;
	}


	if (IsLocalPlayerController())
	{
		if (LobbyHUDClass)
		{
			ZLOG(Error, TEXT("CreateLobbyWidget"));
			LobbyHUD = CreateWidget<UZLobbyWidget>(this, LobbyHUDClass);
			if (LobbyHUD)
			{
				LobbyHUD->AddToViewport();
				bShowMouseCursor = true;
				SetInputMode(FInputModeGameAndUI());
				ServerIsHost();
				//if (HasAuthority())
				//{
				//	LobbyHUD->PlayFadeInStartButtonAnim();
				//}

				auto MyGameState = GetWorld()->GetGameState<AZLobbyGameState>();
				if (MyGameState)
				{
					UpdateConnectNumber(MyGameState->GetConnectNumber());
				}
			}
		}
	}


}



void AZLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(AZLobbyPlayerController, bIsHost, COND_OwnerOnly);
}

void AZLobbyPlayerController::UpdateConnectNumber(int32 NewNumber)
{
	if (IsLocalPlayerController())
	{
		if (LobbyHUD)
		{
			LobbyHUD->UpdateConnectNumber(NewNumber);
		}
	}

}

void AZLobbyPlayerController::UpdatePlayerName(const FString & PlayerName, bool bErase)
{
	if (IsLocalPlayerController())
	{
		if (LobbyHUD)
		{
			if (bErase)
			{
				LobbyHUD->DeletePlayerName(PlayerName);
			}
			else
			{
				LobbyHUD->UpdatePlayerName(PlayerName);
			}

		}
	}

}

bool AZLobbyPlayerController::ClientReceiveChat_Validate(const FString & PlayerName, const FString& RecvChat)
{
	return true;
}

void AZLobbyPlayerController::ClientReceiveChat_Implementation(const FString & PlayerName, const FString& RecvChat)
{
	if (nullptr == LobbyHUD)
	{
		ZLOG(Error, TEXT("Lobby UserHUD null."));
		return;
	}

	LobbyHUD->UpdateChatBox(PlayerName, RecvChat);

}

bool AZLobbyPlayerController::ClientUpdateJoinPlayer_Validate(const FString & JoinPlayer, bool bErase)
{
	return true;
}

void AZLobbyPlayerController::ClientUpdateJoinPlayer_Implementation(const FString & JoinPlayer, bool bErase)
{
	UpdatePlayerName(JoinPlayer, bErase);
}

bool AZLobbyPlayerController::ClientActiveStartButton_Validate()
{
	return true;
}

void AZLobbyPlayerController::ClientActiveStartButton_Implementation()
{
	if (LobbyHUD)
	{
		LobbyHUD->PlayFadeInStartButtonAnim();
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

bool AZLobbyPlayerController::ServerStartGame_Validate()
{
	return true;
}

void AZLobbyPlayerController::ServerStartGame_Implementation()
{
	ZLOG_S(Error);
	auto MyGameMode = Cast<AZLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		MyGameMode->StartGame();
	}
	else
	{
		ZLOG(Error, TEXT("No game mode.."));
	}
}

bool AZLobbyPlayerController::ServerIsHost_Validate()
{
	return true;
}

void AZLobbyPlayerController::ServerIsHost_Implementation()
{
	if (bIsHost)
	{
		ClientActiveStartButton();
	}

}

void AZLobbyPlayerController::OnRep_IsHost()
{
	if (bIsHost)
	{
		if (LobbyHUD)
		{
			LobbyHUD->PlayFadeInStartButtonAnim();
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


