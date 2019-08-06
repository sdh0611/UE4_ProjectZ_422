// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserHUD.h"
#include "ZGameState.h"
#include "ZInventoryWidget.h"
#include "ZShopWidget.h"
#include "ZHPBarWidget.h"
#include "ZCurrentWeaponInfoWidget.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "ZCharacterItemStatusComponent.h"
#include "Components/TextBlock.h"

void UZUserHUD::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewInventoryWidget = Cast<UZInventoryWidget>(GetWidgetFromName(TEXT("UI_Inventory")));
	check(nullptr != NewInventoryWidget);
	InventoryWidget = NewInventoryWidget;

	auto NewShopWidget = Cast<UZShopWidget>(GetWidgetFromName(TEXT("UI_Shop")));
	check(nullptr != NewShopWidget);
	ShopWidget = NewShopWidget;

	auto NewHPBarWidget = Cast<UZHPBarWidget>(GetWidgetFromName(TEXT("UI_HPBar")));
	check(nullptr != NewHPBarWidget);
	HPBarWidget = NewHPBarWidget;

	auto NewCurrnetWeaponInfoWidget = Cast<UZCurrentWeaponInfoWidget>(GetWidgetFromName(TEXT("UI_CurrentWeaponInfo")));
	check(nullptr != NewCurrnetWeaponInfoWidget);
	CurrentWeaponInfoWidget = NewCurrnetWeaponInfoWidget;

	auto NewCurrentMoneyInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrnetMoney")));
	check(nullptr != NewCurrentMoneyInfoText);
	CurrentMoneyInfoText = NewCurrentMoneyInfoText;

	auto NewRemainTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_RemainTimeValue")));
	check(nullptr != NewRemainTimeText);
	RemainTimeText = NewRemainTimeText;

	auto NewTotalWaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_TotalWaveValue")));
	check(nullptr != NewTotalWaveText);
	TotalWaveText = NewTotalWaveText;

	auto NewCurrentWaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrentWaveValue")));
	check(nullptr != NewCurrentWaveText);
	CurrentWaveText = NewCurrentWaveText;

	auto NewCurrentNumZombiesText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_NumZombiesValue")));
	check(nullptr != NewCurrentNumZombiesText);
	CurrentNumZombiesText = NewCurrentNumZombiesText;
	

	auto Player = Cast<AZCharacter>(GetOwningPlayerPawn());
	check(nullptr != Player);
	HPBarWidget->BindStatus(Player->GetStatusComponent());
	UpdateCurrentMoneyInfo(Player->GetItemStatusComponent()->GetCurrentMoney());

	/* 이부분 UI 제거될 때 Delegate binding 어떻게 제거할지 고민해봐야함. */
	auto GameState = Cast<AZGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->OnTimeUpdate.AddUObject(this, &UZUserHUD::UpdateRemainTime);
	}

}

void UZUserHUD::UpdateCurrentMoneyInfo(int32 NewMoney)
{
	CurrentMoneyInfoText->SetText(FText::FromString(FString::FromInt(NewMoney)));
}

void UZUserHUD::UpdateRemainTime(float NewTime)
{
	FString Time = FString::Printf(TEXT("%.1f"), NewTime);
	RemainTimeText->SetText(FText::FromString(Time));
}

void UZUserHUD::UpdateCurrentWave(int32 NewCurrentWave)
{
	FString CurrentWave = FString::FromInt(NewCurrentWave);
	CurrentWaveText->SetText(FText::FromString(CurrentWave));
}

void UZUserHUD::UpdateNumZombies(int32 NewValue)
{
	FString CurrentNumZombies = FString::FromInt(NewValue);
	CurrentNumZombiesText->SetText(FText::FromString(CurrentNumZombies));
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

UZCurrentWeaponInfoWidget * const UZUserHUD::GetCurrentWeaponInfoWidget() const
{
	return CurrentWeaponInfoWidget;
}
