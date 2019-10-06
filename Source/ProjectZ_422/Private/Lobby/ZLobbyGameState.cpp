// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyGameState.h"
#include "ZLobbyPlayerController.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"


void AZLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZLobbyGameState, ConnectNumber);

}

void AZLobbyGameState::SetConnectNumber(int32 NewNumber)
{
	if (NewNumber < 0)
	{
		return;
	}

	ConnectNumber = NewNumber;

	OnRep_ConnectNumber();

}

int32 AZLobbyGameState::GetConnectNumber() const
{
	return ConnectNumber;
}

void AZLobbyGameState::OnRep_ConnectNumber()
{
	auto LobbyPC = Cast<AZLobbyPlayerController>(*GetWorld()->GetPlayerControllerIterator());
	if (LobbyPC && LobbyPC->IsLocalPlayerController())
	{
		LobbyPC->UpdateConnectNumber(ConnectNumber);
	}
}
