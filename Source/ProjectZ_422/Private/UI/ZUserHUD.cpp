// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserHUD.h"
#include "ZInventoryWidget.h"
#include "ZShopWidget.h"

void UZUserHUD::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewInventoryWidget = Cast<UZInventoryWidget>(GetWidgetFromName(TEXT("UI_Inventory")));
	check(nullptr != NewInventoryWidget);
	InventoryWidget = NewInventoryWidget;

	auto NewShopWidget = Cast<UZShopWidget>(GetWidgetFromName(TEXT("UI_Shop")));
	check(nullptr != NewShopWidget);
	ShopWidget = NewShopWidget;

}

void UZUserHUD::DrawInventoryWidget()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(InventoryWidget->GetCachedWidget());

	auto PlayerController = GetOwningPlayer();

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	bIsInventoryOnScreen = true;

}

void UZUserHUD::DrawShopWidget()
{
	ShopWidget->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(InventoryWidget->GetCachedWidget());

	auto PlayerController = GetOwningPlayer();

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	bIsShopOnScreen = true;

}

void UZUserHUD::RemoveInventoryWidget()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

	auto PlayerController = GetOwningPlayer();

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;

	bIsInventoryOnScreen = false;
}

void UZUserHUD::RemoveShopWidget()
{
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);

	auto PlayerController = GetOwningPlayer();

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;

	bIsShopOnScreen = false;
}

bool UZUserHUD::IsInventoryOnScreen() const
{
	return bIsInventoryOnScreen;
}

bool UZUserHUD::IsShopWidgetOnScreen() const
{
	return bIsShopOnScreen;
}

UZInventoryWidget * const UZUserHUD::GetInventoryWidget() const
{
	return InventoryWidget;
}

UZShopWidget * const UZUserHUD::GetShopWidget() const
{
	return ShopWidget;
}
