// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitlePlayerController.h"
#include "..\..\Public\Title\ZTitlePlayerController.h"
#include "ZGameInstance.h"
#include "Title/ZTitleWidget.h"
#include "Title/ZTitleGameMode.h"
#include "Json.h"

void AZTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		bShowMouseCursor = true;

		if (TitleWidgetClass)
		{
			TitleWidget = CreateWidget<UZTitleWidget>(this, TitleWidgetClass);

			if (TitleWidget)
			{
				TitleWidget->AddToViewport();

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(TitleWidget->GetCachedWidget());
				SetInputMode(InputMode);
			}
		}
	}

}



void AZTitlePlayerController::OnLoginServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	ZLOG_S(Error);
	if (bWasSuccessful)
	{
		ZLOG_S(Error);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool bResult = JsonObject->GetBoolField("result");
			if (bResult)
			{
				FString NewNickname = JsonObject->GetStringField("nickname");
				ZLOG(Error, TEXT("Nickname : %s"), *NewNickname);
				ClientReceiveLoginSuccess(NewNickname);
				return;
			}
			else
			{
				ZLOG(Error, TEXT("Result fail.."));
				return;
			}
		}
		else
		{
			ZLOG(Error, TEXT("Deserialize fail.."));
		}

	}

	ClientRecevieLoginFail();

}

bool AZTitlePlayerController::ClientReceiveLoginSuccess_Validate(const FString& NewNickname)
{
	return true;
}

void AZTitlePlayerController::ClientReceiveLoginSuccess_Implementation(const FString& NewNickname)
{
	//auto MyGameInstance = GetGameInstance<UZGameInstance>();
	//if (MyGameInstance)
	//{
	//	MyGameInstance->SetUserNickname(NewNickname);
	//}

	if (IsLocalPlayerController())
	{
		if (TitleWidget)
		{
			TitleWidget->SetIsVerified(true);
		}
	}

}

bool AZTitlePlayerController::ClientRecevieLoginFail_Validate()
{
	return true;
}

void AZTitlePlayerController::ClientRecevieLoginFail_Implementation()
{
	if (IsLocalPlayerController())
	{
		if (TitleWidget)
		{
			TitleWidget->SetIDInputWidgetErrorText(TEXT("일치하는 정보가 없습니다."));
			TitleWidget->SetIsVerified(false);
		}
	}

}

