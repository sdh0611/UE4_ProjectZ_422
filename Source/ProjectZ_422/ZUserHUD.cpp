// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserHUD.h"
#include "ZInventoryWidget.h"


void UZUserHUD::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewInventoryWidget = Cast<UZInventoryWidget>(GetWidgetFromName(TEXT("UI_Inventory")));
	check(nullptr != NewInventoryWidget);
	InventoryWidget = NewInventoryWidget;

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

bool UZUserHUD::IsInventoryOnScreen() const
{
	return bIsInventoryOnScreen;
}

UZInventoryWidget * const UZUserHUD::GetInventoryWidget() const
{
	return InventoryWidget;
}
