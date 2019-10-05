// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitlePlayerController.h"
#include "..\..\Public\Title\ZTitlePlayerController.h"
#include "Title/ZTitleWidget.h"
#include "Title/ZTitleGameMode.h"

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

bool AZTitlePlayerController::ServerRequestLogin_Validate(const FString & URL, const FString & UserID, const FString & UserPW)
{
	return true;
}

void AZTitlePlayerController::ServerRequestLogin_Implementation(const FString & URL, const FString & UserID, const FString & UserPW)
{
	auto MyGameMode = GetWorld()->GetAuthGameMode<AZTitleGameMode>();
	if (nullptr == MyGameMode)
	{
		ZLOG(Error, TEXT("Failed to get game mode.."));
		return;
	}

	MyGameMode->HttpPost(URL, UserID, UserPW, 
		FHttpRequestCompleteDelegate::CreateUObject(this, &AZTitlePlayerController::OnLoginServerResponseReceived));


}

void AZTitlePlayerController::OnLoginServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		

	}


}
