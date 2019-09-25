// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerController.h"
#include "ZPlayerState.h"
#include "ZHUD.h"
#include "ZPlayerCameraManager.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZUserHUD.h"


AZPlayerController::AZPlayerController()
{
	PlayerCameraManagerClass = AZPlayerCameraManager::StaticClass();

}

void AZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == UserHUDClass)
	{
		ZLOG(Error, TEXT("UserHUDClass null."));
		return;
	}

	if (IsLocalPlayerController())
	{
		UserHUD = CreateWidget<UZUserHUD>(this, UserHUDClass);
		if (nullptr == UserHUD)
		{
			ZLOG(Error, TEXT("Failed to create UserHUD."));
			return;
		}

		UserHUD->AddToViewport();
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

}

void AZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("ToggleInventory"), IE_Pressed, this, &AZPlayerController::ToggleInventory);

	InputComponent->BindAction(TEXT("ToggleInGameMenu"), IE_Pressed, this, &AZPlayerController::ToggleInGameMenu);
	InputComponent->BindAction(TEXT("RemoveWidgetFromTop"), IE_Pressed, this, &AZPlayerController::RemoveWidgetFromTop);

}

UZUserHUD * const AZPlayerController::GetUserHUD() const
{
	return UserHUD;
}

AZHUD * const AZPlayerController::GetZHUD() const
{
	return Cast<AZHUD>(GetHUD());
}

UZCharacterItemStatusComponent * const AZPlayerController::GetCharacterItemStatusComponent() const
{
	auto Chracter = Cast<AZCharacter>(GetPawn());
	if (Chracter)
	{
		return Chracter->GetItemStatusComponent();
	}

	return nullptr;
}

void AZPlayerController::ToggleInventory()
{
	if (GetUserHUD())
	{
		GetUserHUD()->ToggleInventoryWidget();
	}

}

void AZPlayerController::ToggleInGameMenu()
{
	if (GetUserHUD())
	{
		GetUserHUD()->ToggleInGameMenuWIdget();
	}

}

void AZPlayerController::RemoveWidgetFromTop()
{
	if (GetUserHUD())
	{
		GetUserHUD()->RemoveWidgetFromTop();
	}

}
