// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "ZClientGameInstance.h"
#include "ZServerGameInstance.h"
#include "ZBaseGameMode.h"
#include "ZPlayerState.h"
#include "Json.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UI/ZGameViewportClient.h"

void AZBasePlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

}

void AZBasePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//auto MyGameInstnace = GetGameInstance<UZGameInstance>();
	//if (MyGameInstnace)
	//{
	//	if (MyGameInstnace->DestroySession())
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("DestroySession success")));
	//		//if (IsLocalPlayerController())
	//		//{
	//		//	//PC->ClientTravel(TEXT("StartMenu"), ETravelType::TRAVEL_Absolute);
	//		//	//GetWorld()->ServerTravel(TEXT("StartMenu"));
	//		//}
	//	}
	//}

	Super::EndPlay(EndPlayReason);

}

void AZBasePlayerController::PreClientTravel(const FString & PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("PreClientTravel.")));
	auto MyGameInstnace = GetGameInstance<UZGameInstance>();
	if (MyGameInstnace)
	{
		MyGameInstnace->ShowLoadingScreen();
	}

	//auto MyGameViewport = Cast<UZGameViewportClient>(GetWorld()->GetGameViewport());
	//if (MyGameViewport)
	//{
	//	MyGameViewport->ShowLoadingScreen();
	//}

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("PreClientTravel.")));
	//if (IsLocalPlayerController() && LoadingWidget)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("ShowLoadingScreen.")));
	//	LoadingWidget->AddToViewport();
	//}

	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);

}

//void AZBasePlayerController::RemoveAllWidget()
//{
//	if (IsLocalPlayerController())
//	{
//		TArray<UUserWidget*> Widgets;
//		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UUserWidget::StaticClass());
//		for (auto& Widget : Widgets)
//		{
//			Widget->RemoveFromParent();
//		}
//	}
//}

//void AZBasePlayerController::ShowLoadingWidget(bool bShow)
//{
//	if (!IsLocalPlayerController())
//	{
//		return;
//	}
//
//	if (LoadingWidget)
//	{
//		ESlateVisibility Visibility = (bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
//		LoadingWidget->SetVisibility(Visibility);
//	}
//
//}

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

bool AZBasePlayerController::ClientRemoveAllWidget_Validate()
{
	return true;
}

void AZBasePlayerController::ClientRemoveAllWidget_Implementation()
{
	//RemoveAllWidget();
}



bool AZBasePlayerController::ClientReceiveRemovePlayerSession_Validate()
{
	return true;
}

void AZBasePlayerController::ClientReceiveRemovePlayerSession_Implementation()
{
	auto MyGameInstance = GetGameInstance<UZClientGameInstance>();
	if (MyGameInstance)
	{
		ServerRemovePlayerSession(MyGameInstance->GetPlayerSessionID());
	}

}

bool AZBasePlayerController::ClientReceiveAcceptPlayerSession_Validate()
{
	return true;
}

void AZBasePlayerController::ClientReceiveAcceptPlayerSession_Implementation()
{
	auto MyGameInstance = GetGameInstance<UZClientGameInstance>();
	if (MyGameInstance)
	{
		ServerAcceptPlayerSession(MyGameInstance->GetPlayerSessionID());
	}

}

bool AZBasePlayerController::ServerReceiveUserName_Validate(const FString & UserName)
{
	return true;
}

void AZBasePlayerController::ServerReceiveUserName_Implementation(const FString & UserName)
{
	OnReceiveUserName(UserName);

}

bool AZBasePlayerController::ServerRemovePlayerSession_Validate(const FString & PlayerSessionID)
{
	return true;
}

void AZBasePlayerController::ServerRemovePlayerSession_Implementation(const FString & PlayerSessionID)
{
	auto MyGameInstance = GetGameInstance<UZServerGameInstance>();
	if (MyGameInstance)
	{
		MyGameInstance->RemovePlayerSession(PlayerSessionID);
	}

}

bool AZBasePlayerController::ServerAcceptPlayerSession_Validate(const FString & PlayerSessionID)
{
	return true;
}

void AZBasePlayerController::ServerAcceptPlayerSession_Implementation(const FString & PlayerSessionID)
{
	auto MyGameInstance = GetGameInstance<UZServerGameInstance>();
	if (MyGameInstance)
	{
		MyGameInstance->AcceptPlayerSession(PlayerSessionID);
	}

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

	//if (IsLocalPlayerController())
	//{
	//	LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
	//	check(LoadingWidget);
	//}

	//if (IsLocalPlayerController())
	//{
	//	TArray<UUserWidget*> Widgets;
	//	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UUserWidget::StaticClass());
	//	for (auto& Widget : Widgets)
	//	{
	//		Widget->RemoveFromParent();
	//	}
	//}

}
