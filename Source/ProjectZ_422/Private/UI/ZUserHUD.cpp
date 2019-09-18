// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserHUD.h"
#include "ZUserWidget.h"
#include "ZGameState.h"
#include "ZGameMode.h"
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

	/* Dead menu widget */
	auto NewDeadMenuWidget = Cast<UZUserWidget>(GetWidgetFromName(TEXT("UI_EndGame")));
	check(nullptr != NewDeadMenuWidget);
	EndGameMenuWidget = NewDeadMenuWidget;
	EndGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);

	/* InGame menu widget */
	auto NewInGameMenuWidget = Cast<UZUserWidget>(GetWidgetFromName(TEXT("UI_InGame")));
	check(nullptr != NewInGameMenuWidget);
	InGameMenuWidget = NewInGameMenuWidget;
	InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);


	/* Money text */
	auto NewCurrentMoneyInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrnetMoney")));
	check(nullptr != NewCurrentMoneyInfoText);
	CurrentMoneyInfoText = NewCurrentMoneyInfoText;

	/* Time text */
	auto NewRemainTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_RemainTimeValue")));
	check(nullptr != NewRemainTimeText);
	RemainTimeText = NewRemainTimeText;

	/* Phase text */
	auto NewPhaseText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Phase")));
	check(nullptr != NewPhaseText);
	PhaseText = NewPhaseText;

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
			//CallFadeInAnim();
			break;
		}
		default:
		{
			PhaseText->SetText(FText::GetEmpty());
			break;
		}

	}
}

void UZUserHUD::ToggleInventoryWidget()
{
	if (ESlateVisibility::Hidden == InventoryWidget->GetVisibility())
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
	if (ESlateVisibility::Hidden == ShopWidget->GetVisibility())
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
	if (ESlateVisibility::Hidden == InGameMenuWidget->GetVisibility())
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
	if (ESlateVisibility::Hidden == InGameMenuWidget->GetVisibility())
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
	bIsShopOnScreen = true;
}

void UZUserHUD::DrawEndGameMenuWidget()
{
	AddWidgetToList(EndGameMenuWidget);
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

	bIsShopOnScreen = false;
}

void UZUserHUD::RemoveEndGameMenuWidget()
{
	RemoveWidgetFromList(EndGameMenuWidget);

}

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

void UZUserHUD::AddWidgetToList(UZUserWidget * Widget)
{
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
}

void UZUserHUD::RemoveWidgetFromList(UZUserWidget* Widget)
{
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

}

bool UZUserHUD::IsDrawWidgetListEmpty()
{
	return DrawWidgetList.Num() < 1;
}
