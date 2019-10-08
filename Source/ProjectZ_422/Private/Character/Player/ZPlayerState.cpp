// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerState.h"
#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "UnrealNetwork.h"
#include "ZLobbyPlayerController.h"
#include "Engine/World.h"

AZPlayerState::AZPlayerState()
{
	Kills = 0;

}

void AZPlayerState::AddKill()
{
	++Kills;
}

void AZPlayerState::AdjustScore(int32 NewScore)
{
	CurScore = FMath::Clamp<int32>(CurScore + NewScore, 0, CurScore + NewScore);
}

void AZPlayerState::SetPlayerIndex(int32 NewIndex)
{
	PlayerIndex = NewIndex;
}

void AZPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//if (!HasAuthority())
	//{
	//	auto LobbyPC = Cast<AZLobbyPlayerController>(*(GetWorld()->GetPlayerControllerIterator()));
	//	if (nullptr == LobbyPC)
	//	{
	//		ZLOG(Error, TEXT("Invalid PC.."));
	//		return;
	//	}

	//	LobbyPC->UpdatePlayerName(GetPlayerName(), false);
	//}

}

void AZPlayerState::SetPlayerName(const FString & S)
{
	Super::SetPlayerName(S);

	//MulticastSetPlayerName(S);
	//OnRep_PlayerName();
}

void AZPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZPlayerState, PlayerIndex);

}

void AZPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	//auto LobbyPC = Cast<AZLobbyPlayerController>(*(GetWorld()->GetPlayerControllerIterator()));
	//if (nullptr == LobbyPC)
	//{
	//	ZLOG(Error, TEXT("Invalid PC.."));
	//	return;
	//}

	//LobbyPC->UpdatePlayerName(GetPlayerName(), false);


}
