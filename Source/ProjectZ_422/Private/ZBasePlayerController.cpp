// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "ZGameInstance.h"
#include "ZBaseGameMode.h"
#include "ZPlayerState.h"
#include "Json.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

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

void AZBasePlayerController::RemoveAllWidget()
{
	if (IsLocalPlayerController())
	{
		TArray<UUserWidget*> Widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UUserWidget::StaticClass());
		for (auto& Widget : Widgets)
		{
			Widget->RemoveFromParent();
		}
	}
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

bool AZBasePlayerController::ClientRemoveAllWidget_Validate()
{
	return true;
}

void AZBasePlayerController::ClientRemoveAllWidget_Implementation()
{
	RemoveAllWidget();
}

bool AZBasePlayerController::ClientDestroySession_Validate()
{
	return true;
}

void AZBasePlayerController::ClientDestroySession_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ClientDestorySessiond"));
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		MyGameInstance->DestroySession();
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

	if (IsLocalPlayerController())
	{
		TArray<UUserWidget*> Widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UUserWidget::StaticClass());
		for (auto& Widget : Widgets)
		{
			Widget->RemoveFromParent();
		}
	}

}
