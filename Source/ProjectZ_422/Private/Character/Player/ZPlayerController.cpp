// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerController.h"
#include "ZPlayerState.h"
#include "ZHUD.h"
#include "ZPlayerCameraManager.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZUserHUD.h"
#include "ZShopWidget.h"
#include "ZShop.h"
#include "ZGameMode.h"
#include "ZGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"


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

bool AZPlayerController::Buy_Validate(int32 BuyItemShopID, int32 Quantity)
{
	return true;
}

void AZPlayerController::Buy_Implementation(int32 BuyItemShopID, int32 Quantity)
{
	auto MyGameMode = GetWorld()->GetAuthGameMode<AZGameMode>();
	if (MyGameMode)
	{
		auto Shop = MyGameMode->GetShop();
		if (Shop && Shop->bIsShopOpen)
		{
			Shop->Buy(this, BuyItemShopID, Quantity);
		}

	}
}

void AZPlayerController::OpenShop()
{
	//if (nullptr == Shop)
	//{
	//	ZLOG(Error, TEXT("Shop is null."));
	//	return;
	//}

	///* 로컬에서만 실행. */
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("PC OpenShop."));
	ConstructShopWidget();
	//Shop->OpenShop(this);
	


}

void AZPlayerController::ConstructShopWidget()
{
	ZLOG_S(Warning);
	if(nullptr == UserHUD)
	{
		return;
	}

	/* 상점 전체 틀 그림. */
	UserHUD->DrawShopWidget();

	/* 상점 아이템들을 그리기 위한 준비. */
	auto ShopWidget = Cast<UZShopWidget>(UserHUD->GetShopWidget());
	if (nullptr == ShopWidget)
	{
		return;
	}

	auto MyPawn = Cast<AZCharacter>(GetPawn());
	if (nullptr == MyPawn)
	{
		return;
	}

	auto ItemStatusComponent = MyPawn->GetItemStatusComponent();
	if (nullptr == ItemStatusComponent)
	{
		ZLOG(Error, TEXT("ItemStatusComponent not exist.."));
		return;
	}

	UDataTable* ShopItemDataTable = GetGameInstance<UZGameInstance>()->GetShopItemDataTable();

	ShopWidget->ConstructBuyWidget(ShopItemDataTable);
	ShopWidget->ConstructSellWidget(ItemStatusComponent->GetItemList());



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
