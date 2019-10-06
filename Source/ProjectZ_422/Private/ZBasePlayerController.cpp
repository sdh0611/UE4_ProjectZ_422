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

bool AZBasePlayerController::ClientRequestLogout_Validate()
{
	return true;
}

void AZBasePlayerController::ClientRequestLogout_Implementation()
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (nullptr == MyGameInstance)
	{
		ZLOG(Error, TEXT("Invalid game instance.."));
		return;
	}
	
	MyGameInstance->HttpPostLogout(TEXT("127.0.0.1:8000"),
		FHttpRequestCompleteDelegate::CreateUObject(this, &AZBasePlayerController::OnLogoutResponseReceived));

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
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Nickname : %s"), *MyGameInstance->GetUserNickname()));
		ServerReceiveSetUserName(MyGameInstance->GetUserNickname());
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

void AZBasePlayerController::OnLogoutResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		ZLOG_S(Error);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool bResult = JsonObject->GetBoolField("result");
			if (!bResult)
			{
				ZLOG(Error, TEXT("Logout fail.."));
				return;
			}

			auto MyGameInstance = GetGameInstance<UZGameInstance>();
			if (nullptr == MyGameInstance)
			{
				ZLOG(Error, TEXT("Invalid game instance.."));
				return;
			}

			MyGameInstance->SetUserID(TEXT(""));
			MyGameInstance->SetUserNickname(TEXT(""));
			MyGameInstance->bIsVerified = false;
		}
		else
		{
			ZLOG(Error, TEXT("Deserialize fail.."));
		}

	}

}

