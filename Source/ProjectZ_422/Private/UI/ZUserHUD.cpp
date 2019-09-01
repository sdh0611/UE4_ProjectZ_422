// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserHUD.h"
#include "ZGameState.h"
#include "ZInventoryWidget.h"
#include "ZShopWidget.h"
#include "ZHPBarWidget.h"
#include "ZCurrentWeaponInfoWidget.h"
#include "ZInputNumberWidget.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "ZCharacterItemStatusComponent.h"
#include "Components/TextBlock.h"

UZUserHUD::UZUserHUD(const FObjectInitializer & ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
}

void UZUserHUD::NativeConstruct()
{
	Super::NativeConstruct();

	/* Inventory */
	auto NewInventoryWidget = Cast<UZInventoryWidget>(GetWidgetFromName(TEXT("UI_Inventory")));
	check(nullptr != NewInventoryWidget);
	InventoryWidget = NewInventoryWidget;

	/* Shop widget */
	auto NewShopWidget = Cast<UZShopWidget>(GetWidgetFromName(TEXT("UI_Shop")));
	check(nullptr != NewShopWidget);
	ShopWidget = NewShopWidget;

	/* HPBar widget */
	auto NewHPBarWidget = Cast<UZHPBarWidget>(GetWidgetFromName(TEXT("UI_HPBar")));
	check(nullptr != NewHPBarWidget);
	HPBarWidget = NewHPBarWidget;

	/* Weapon info widget */
	auto NewCurrnetWeaponInfoWidget = Cast<UZCurrentWeaponInfoWidget>(GetWidgetFromName(TEXT("UI_CurrentWeaponInfo")));
	check(nullptr != NewCurrnetWeaponInfoWidget);
	CurrentWeaponInfoWidget = NewCurrnetWeaponInfoWidget;

	/* Input number widget */
	auto NewInputNumberWidget = Cast<UZInputNumberWidget>(GetWidgetFromName(TEXT("UI_InputNumber")));
	check(nullptr != NewInputNumberWidget);
	InputNumberWidget = NewInputNumberWidget;


	/* Money text */
	auto NewCurrentMoneyInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrnetMoney")));
	check(nullptr != NewCurrentMoneyInfoText);
	CurrentMoneyInfoText = NewCurrentMoneyInfoText;

	/* Time text */
	auto NewRemainTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_RemainTimeValue")));
	check(nullptr != NewRemainTimeText);
	RemainTimeText = NewRemainTimeText;

	/* Total wave text */
	auto NewTotalWaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_TotalWaveValue")));
	check(nullptr != NewTotalWaveText);
	TotalWaveText = NewTotalWaveText;

	/* Current wave text */
	auto NewCurrentWaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrentWaveValue")));
	check(nullptr != NewCurrentWaveText);
	CurrentWaveText = NewCurrentWaveText;

	/* NumZombies text */
	auto NewCurrentNumZombiesText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_NumZombiesValue")));
	check(nullptr != NewCurrentNumZombiesText);
	CurrentNumZombiesText = NewCurrentNumZombiesText;
	

	/* HPBar에 StatusComponent 바인딩 */
	auto Player = Cast<AZCharacter>(GetOwningPlayerPawn());
	check(nullptr != Player);
	HPBarWidget->BindStatus(Player->GetStatusComponent());

	/* Money Info 업데이트 */
	UpdateCurrentMoneyInfo(Player->GetItemStatusComponent()->GetCurrentMoney());


	/* 
		이부분 UI 제거될 때 Delegate binding 어떻게 제거할지 고민해봐야함. 
		->NativeDestruct() 호출될 때 제거함.
	*/
	auto MyGameState = GetWorld()->GetGameState<AZGameState>();
	if (MyGameState)
	{
		UpdateRemainTime(MyGameState->GetRemainTime());
		UpdateCurrentWave(MyGameState->GetCurrentWave());
		UpdateNumZombies(MyGameState->GetCurrentNumZombies());
		TotalWaveText->SetText(FText::FromString(FString::FromInt(MyGameState->GetTotalWave())));
	}

}

void UZUserHUD::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UZUserHUD::NativeDestruct()
{
	Super::NativeDestruct();

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

UZInputNumberWidget * const UZUserHUD::GetInputNumberWidget() const
{
	return InputNumberWidget;
}
