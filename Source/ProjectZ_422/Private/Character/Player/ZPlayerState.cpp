// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerState.h"
#include "ZBasePlayerController.h"
#include "ZCharacter.h"

AZPlayerState::AZPlayerState()
{
	Kills = 0;

}

void AZPlayerState::MulticastSetPlayerName_Implementation(const FString & NewName)
{
	auto MyPawn = Cast<AZCharacter>(GetPawn());
	if (nullptr == MyPawn)
	{
		ZLOG(Error, TEXT("Invalid pawn.."));
		return;
	}

	MyPawn->SetUserName(NewName);
}

void AZPlayerState::AddKill()
{
	++Kills;
}

void AZPlayerState::AdjustScore(int32 NewScore)
{
	CurScore = FMath::Clamp<int32>(CurScore + NewScore, 0, CurScore + NewScore);
}

void AZPlayerState::SetPlayerName(const FString & S)
{
	Super::SetPlayerName(S);

	//MulticastSetPlayerName(S);
	OnRep_PlayerName();
}

void AZPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	auto MyPawn = Cast<AZCharacter>(GetPawn());
	if (nullptr == MyPawn)
	{
		ZLOG(Error, TEXT("Invalid pawn.."));
		return;
	}

	MyPawn->SetUserName(GetPlayerName());

}
