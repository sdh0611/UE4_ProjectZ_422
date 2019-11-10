// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseGameMode.h"
#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


AZBaseGameMode::AZBaseGameMode()
{

	
}

void AZBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AZBaseGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto BasePC = Cast<AZBasePlayerController>(NewPlayer);
	if (BasePC)
	{
		BasePC->ClientReceiveGetUserName();
		++ConnectNumber;
	}

}

void AZBaseGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	--ConnectNumber;
	//if (ConnectNumber < 1)
	//{
	//	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	//}
}

void AZBaseGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (EEndPlayReason::LevelTransition == EndPlayReason)
	{
		ZLOG(Error, TEXT("Level transition!"));
	}

	//for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	//{
	//	auto PC = Cast <AZBasePlayerController>(*Iter);
	//	if (PC)
	//	{
	//		PC->ClientDestroySession();
	//	}
	//}
	
	Super::EndPlay(EndPlayReason);
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

int32 AZBaseGameMode::GetConnectNumber() const
{
	return ConnectNumber;
}
