// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseGameMode.h"
#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "Engine/World.h"

void AZBaseGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	//{
	//	auto BasePC = Cast<AZBasePlayerController>(*Iter);
	//	if (BasePC)
	//	{
	//		BasePC->ClientReceiveSetUserName();
	//	}
	//}
}

void AZBaseGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

}

void AZBaseGameMode::UpdatePlayersName()
{
	//for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	//{
	//	auto Character = Cast<AZCharacter>(Iter->Get()->GetPawn());
	//	if (::IsValid(Character))
	//	{
	//		Character->MulticastUpdatePlayerName();
	//	}

	//	//auto BasePC = Cast<AZBasePlayerController>(*Iter);
	//	//if (BasePC)
	//	//{
	//	//	BasePC->ClientReceiveSetUserName();
	//	//}
	//}
}
