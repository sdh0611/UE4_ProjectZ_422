// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserHUD.h"
#include "ZUserWidget.h"
#include "ZGameState.h"
#include "ZGameMode.h"
#include "ZInventoryWidget.h"
#include "ZShopWidget.h"
#include "ZHPBarWidget.h"
#include "ZEndGameWidget.h"
#include "ZCurrentWeaponInfoWidget.h"
#include "ZInputNumberWidget.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "ZCharacterItemStatusComponent.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

UZUserHUD::UZUserHUD(const FObjectInitializer & ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
}

void UZUserHUD::NativeConstruct()
{
	Super::NativeConstruct();

	/* Inventory */
	InventoryWidget = Cast<UZInventoryWidget>(GetWidgetFromName(TEXT("UI_Inventory")));
	check(InventoryWidget);

	/* Shop widget */
	ShopWidget = Cast<UZShopWidget>(GetWidgetFromName(TEXT("UI_Shop")));
	check(ShopWidget);

	/* HPBar widget */
	HPBarWidget = Cast<UZHPBarWidget>(GetWidgetFromName(TEXT("UI_HPBar")));
	check(HPBarWidget);

	/* Weapon info widget */
	CurrentWeaponInfoWidget = Cast<UZCurrentWeaponInfoWidget>(GetWidgetFromName(TEXT("UI_CurrentWeaponInfo")));
	check(CurrentWeaponInfoWidget);

	/* Input number widget */
	InputNumberWidget = Cast<UZInputNumberWidget>(GetWidgetFromName(TEXT("UI_InputNumber")));
	check(InputNumberWidget);

	/* End menu widget */
	EndGameMenuWidget = Cast<UZEndGameWidget>(GetWidgetFromName(TEXT("UI_EndGame")));
	check(EndGameMenuWidget);
	EndGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);

	/* InGame menu widget */
	InGameMenuWidget = Cast<UZUserWidget>(GetWidgetFromName(TEXT("UI_InGame")));
	check(InGameMenuWidget);
	InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);


	/* Money text */
	CurrentMoneyInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrnetMoney")));
	check(CurrentMoneyInfoText);

	/* Time text */
	RemainTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_RemainTimeValue")));
	check(RemainTimeText);

	/* Phase text */
	PhaseText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Phase")));
	check(PhaseText);

	/* Total wave text */
	TotalWaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_TotalWaveValue")));
	check(TotalWaveText);

	/* Current wave text */
	CurrentWaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrentWaveValue")));
	check(CurrentWaveText);

	/* NumZombies text */
	CurrentNumZombiesText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_NumZombiesValue")));
	check(CurrentNumZombiesText);

	/* InteractionInfo */
	InteractionInfo = Cast<UBorder>(GetWidgetFromName(TEXT("InteractionInfo")));
	check(InteractionInfo);

	/* InteractionNameText */
	InteractionNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_InteractionName")));
	check(InteractionNameText);

	/* HPBar에 StatusComponent 바인딩 */
	auto Player = Cast<AZCharacter>(GetOwningPlayerPawn());
	check(nullptr != Player);
	HPBarWidget->BindStatus(Player->GetStatusComponent());

	/* Money Info 업데이트 */
	Player->GetItemStatusComponent()->OnMoneyInfoChange.AddUObject(this, &UZUserHUD::UpdateCurrentMoneyInfo);
	UpdateCurrentMoneyInfo(Player->GetItemStatusComponent()->GetCurrentMoney());

	auto MyGameState = GetWorld()->GetGameState<AZGameState>();
	if (MyGameState)
	{
		UpdateRemainTime(MyGameState->GetRemainTime());
		UpdateCurrentWave(MyGameState->GetCurrentWave());
		UpdateNumZombies(MyGameState->GetCurrentNumZombies());
		TotalWaveText->SetText(FText::FromString(FString::FromInt(MyGameState->GetTotalWave())));
	}

	auto MyGameMode = GetWorld()->GetAuthGameMode<AZGameMode>();
	if (MyGameMode)
	{
		UpdatePhaseText(MyGameMode->GetCurrentGamePhase());
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

void UZUserHUD::UpdatePhaseText(EGamePhase NewPhase)
{
	if (nullptr == PhaseText)
	{
		ZLOG(Error, TEXT("Phase text not exist.."));
		return;
	}

	switch (NewPhase)
	{
	case EGamePhase::HalfTime:
	{
		PhaseText->SetText(FText::FromString(TEXT("전투에 대비하십시오 !")));
		PhaseText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f))));
		break;
		CallFadeAnim();
	}
	case EGamePhase::WaveTime:
	{
		//PhaseText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 0.f))));
		PhaseText->SetText(FText::FromString(TEXT("적들이 몰려옵니다 !")));
		CallWarningAnim();
		break;
	}
	case EGamePhase::Win:
	{
		PhaseText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f))));
		PhaseText->SetText(FText::FromString(TEXT("승리하였습니다 !")));
		CallFadeInAnim();
		break;
	}
	case EGamePhase::Lose:
	{
		PhaseText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f))));
		PhaseText->SetText(FText::FromString(TEXT("패배하였습니다...")));
		CallFadeInAnim();
		break;
	}
	default:
	{
		PhaseText->SetText(FText::GetEmpty());
		break;
	}

	}
}

void UZUserHUD::UpdateInteractionName(const FString & Name)
{
	if (InteractionNameText)
	{
		InteractionNameText->SetText(FText::FromString(Name));
	}

}

void UZUserHUD::ToggleInteractionInfo(bool bShow)
{
	if (bShow)
	{
		if (ESlateVisibility::Visible != InteractionInfo->GetVisibility())
		{
			InteractionInfo->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (ESlateVisibility::Collapsed != InteractionInfo->GetVisibility())
		{
			InteractionInfo->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UZUserHUD::ToggleInventoryWidget()
{
	if (ESlateVisibility::Collapsed == InventoryWidget->GetVisibility())
	{
		DrawInventoryWidget();
	}
	else
	{
		RemoveInventoryWidget();
	}


}

void UZUserHUD::ToggleShopWidget()
{
	if (ESlateVisibility::Collapsed == ShopWidget->GetVisibility())
	{
		DrawShopWidget();
	}
	else
	{
		RemoveShopWidget();
	}
}

void UZUserHUD::ToggleInGameMenuWIdget()
{
	if (ESlateVisibility::Collapsed == InGameMenuWidget->GetVisibility())
	{
		DrawInGameMenuWidget();
	}
	else
	{
		RemoveInGameMenuWidget();
	}
}

void UZUserHUD::ToggleInputNumberWidget()
{
	if (ESlateVisibility::Collapsed == InGameMenuWidget->GetVisibility())
	{
		DrawInputNumberWidget();
	}
	else
	{
		RemoveInputNumberWidget();
	}
}

void UZUserHUD::DrawInventoryWidget()
{
	AddWidgetToList(InventoryWidget);
}

void UZUserHUD::DrawShopWidget()
{
	AddWidgetToList(ShopWidget);
}

void UZUserHUD::DrawDeadMenuWidget()
{
	if (EndGameMenuWidget)
	{
		EndGameMenuWidget->SetEndText(TEXT("YOU DEAD"));
		EndGameMenuWidget->SetVisibility(ESlateVisibility::Visible);

		auto PC = GetOwningPlayer();
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
		}

	}
}

void UZUserHUD::DrawWinMenuWidget()
{
	if (EndGameMenuWidget)
	{
		EndGameMenuWidget->SetEndText(TEXT("VICTORY"));
		EndGameMenuWidget->SetVisibility(ESlateVisibility::Visible);

		auto PC = GetOwningPlayer();
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
		}
	}

}

void UZUserHUD::DrawInGameMenuWidget()
{
	AddWidgetToList(InGameMenuWidget);
}

void UZUserHUD::DrawInputNumberWidget()
{
	AddWidgetToList(InputNumberWidget);
}

void UZUserHUD::RemoveInventoryWidget()
{
	RemoveWidgetFromList(InventoryWidget);
}

void UZUserHUD::RemoveShopWidget()
{
	RemoveWidgetFromList(ShopWidget);
}

//void UZUserHUD::RemoveEndGameMenuWidget()
//{
//	RemoveWidgetFromList(EndGameMenuWidget);
//
//}

void UZUserHUD::RemoveInGameMenuWidget()
{
	RemoveWidgetFromList(InGameMenuWidget);
}

void UZUserHUD::RemoveInputNumberWidget()
{
	RemoveWidgetFromList(InputNumberWidget);
}

void UZUserHUD::RemoveAllWidgetFromScreen()
{
	int32 Size = DrawWidgetList.Num();
	for (int i = 0; i < Size; ++i)
	{
		RemoveWidgetFromTop();
	}
}

void UZUserHUD::RemoveWidgetFromTop()
{
	if (IsDrawWidgetListEmpty())
	{
		return;
	}

	DrawWidgetList.Pop().Get()->OnRemoveScreen();
	if (IsDrawWidgetListEmpty())
	{
		auto PlayerController = GetOwningPlayer();

		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
	}
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

void UZUserHUD::AddWidgetToList(UZUserWidget * Widget)
{
	if (nullptr == Widget)
	{
		return;
	}

	if(DrawWidgetList.Contains(Widget))
	{
		return;
	}

	if (IsDrawWidgetListEmpty())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->GetCachedWidget());

		auto PlayerController = GetOwningPlayer();

		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;

		if (Widget->bIsFocusable)
		{
			Widget->SetUserFocus(PlayerController);
		}

	}

	Widget->OnDrawScreen();
	DrawWidgetList.AddUnique(Widget);

	//if (Widget)
	//{
	//	ZLOG_S(Error);
	//	FInputModeGameAndUI InputMode;
	//	InputMode.SetWidgetToFocus(InventoryWidget->GetCachedWidget());

	//	if (IsDrawWidgetListEmpty())
	//	{
	//		auto PlayerController = GetOwningPlayer();

	//		PlayerController->SetInputMode(InputMode);
	//		PlayerController->bShowMouseCursor = true;
	//		PlayerController->bEnableClickEvents = true;
	//		PlayerController->bEnableMouseOverEvents = true;

	//		if (Widget->bIsFocusable)
	//		{
	//			Widget->SetUserFocus(PlayerController);
	//		}

	//	}

	//	Widget->OnDrawScreen();
	//	
	//	++DrawWidgetNum;
	//}

}

void UZUserHUD::RemoveWidgetFromList(UZUserWidget* Widget)
{
	if (nullptr == Widget)
	{
		return;
	}

	Widget->OnRemoveScreen();
	DrawWidgetList.RemoveSingle(Widget);

	if (IsDrawWidgetListEmpty())
	{
		auto PlayerController = GetOwningPlayer();

		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
	}

	//ZLOG_S(Error);
	//if (Widget)
	//{
	//	ZLOG_S(Error);
	//	--DrawWidgetNum;

	//	Widget->OnRemoveScreen();
	//	if (IsDrawWidgetListEmpty())
	//	{
	//		auto PlayerController = GetOwningPlayer();

	//		PlayerController->SetInputMode(FInputModeGameOnly());
	//		PlayerController->bShowMouseCursor = false;
	//		PlayerController->bEnableClickEvents = false;
	//		PlayerController->bEnableMouseOverEvents = false;
	//	
	//	}

	//}

}

bool UZUserHUD::IsShopWidgetOnScreen() const
{
	return ShopWidget ? (ShopWidget->GetVisibility() == ESlateVisibility::Visible) : false;
}

bool UZUserHUD::IsDrawWidgetListEmpty()
{
	return DrawWidgetList.Num() < 1;
	//return DrawWidgetNum < 1;
}

