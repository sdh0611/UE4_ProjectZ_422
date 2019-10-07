// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "ZGameInstance.h"
#include "ZPlayerState.h"
#include "Json.h"
#include "Kismet/KismetSystemLibrary.h"


void AZBasePlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

}

void AZBasePlayerController::BeginPlay()
{
	Super::BeginPlay();


}

bool AZBasePlayerController::ClientReceiveSetUserName_Validate()
{
	return true;
}

void AZBasePlayerController::ClientReceiveSetUserName_Implementation()
{
	ZLOG_S(Error);
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Nickname : %s"), *MyGameInstance->GetUserNickname()));
		//ServerReceiveSetUserName(MyGameInstance->GetUserNickname());
	}

}

bool AZBasePlayerController::ServerReceiveSetUserName_Validate(const FString & NewName)
{
	return true;
}

void AZBasePlayerController::ServerReceiveSetUserName_Implementation(const FString & NewName)
{
	auto PS = GetPlayerState<AZPlayerState>();
	if (PS)
	{
		PS->SetPlayerName(NewName);
	}

	//auto MyPawn = Cast<AZCharacter>(GetPawn());
	//if (MyPawn)
	//{
	//	MyPawn->MulticastUpdatePlayerName(NewName);
	//}

}
