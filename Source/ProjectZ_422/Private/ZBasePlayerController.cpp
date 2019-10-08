// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "ZGameInstance.h"
#include "ZBaseGameMode.h"
#include "ZPlayerState.h"
#include "Json.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"


void AZBasePlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

}

bool AZBasePlayerController::ClientReceiveGetUserName_Validate()
{
	return true;
}

void AZBasePlayerController::ClientReceiveGetUserName_Implementation()
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (nullptr == MyGameInstance)
	{
		ZLOG(Error, TEXT("Invalid game instance.."));
		return;
	}

	ServerReceiveUserName(MyGameInstance->GetWebConnector().GetUserNickname());

}

bool AZBasePlayerController::ServerReceiveUserName_Validate(const FString & UserName)
{
	return true;
}

void AZBasePlayerController::ServerReceiveUserName_Implementation(const FString & UserName)
{
	OnReceiveUserName(UserName);
	
}

void AZBasePlayerController::OnReceiveUserName(const FString & UserName)
{
	auto MyPS = GetPlayerState<AZPlayerState>();
	if (nullptr == MyPS)
	{
		ZLOG(Error, TEXT("Invalid player state.."));
		return;
	}

	MyPS->SetPlayerName(UserName);

}

void AZBasePlayerController::BeginPlay()
{
	Super::BeginPlay();


}
