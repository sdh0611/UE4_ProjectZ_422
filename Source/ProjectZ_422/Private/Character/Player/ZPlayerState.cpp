// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerState.h"
#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "UnrealNetwork.h"

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
