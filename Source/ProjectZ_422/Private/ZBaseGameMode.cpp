// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseGameMode.h"
#include "ZBasePlayerController.h"
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

	/* DB에 Logout 상태로 변경. */
	//auto MyPC = Cast<AZBasePlayerController>(Exiting);
	//if (MyPC)
	//{
	//	MyPC->ClientRequestLogout();
	//}

}
