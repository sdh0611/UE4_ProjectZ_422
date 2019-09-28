// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopWidget.h"
#include "ZShop.h"
#include "ZShopBuyWidget.h"
#include "ZShopBuyItemWidget.h"
#include "ZShopSellWidget.h"
#include "ZShopSellItemWidget.h"
#include "ZItem.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "ZCharacterItemStatusComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"


void UZShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewExitButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Exit")));
	check(nullptr != NewExitButton);
	ExitButton = NewExitButton;

	auto NewCurrentMoney = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrentMoney")));
	check(NewCurrentMoney);
	CurrentMoney = NewCurrentMoney;

	auto NewShopBuyWidget = Cast<UZShopBuyWidget>(GetWidgetFromName(TEXT("UI_Shop_Buy")));
	check(nullptr != NewShopBuyWidget);
	ShopBuyWidget = NewShopBuyWidget;

	auto NewShopSellWidget = Cast<UZShopSellWidget>(GetWidgetFromName(TEXT("UI_Shop_Sell")));
	check(nullptr != NewShopSellWidget);
	ShopSellWidget = NewShopSellWidget;


	SetVisibility(ESlateVisibility::Hidden);
	ExitButton->OnClicked.AddDynamic(this, &UZShopWidget::OnExitButtonClicked);

	/* ItemStatus에 바인딩. */
	auto Player = GetOwningPlayerPawn<AZCharacter>();
	if (nullptr == Player)
	{
		ZLOG(Error, TEXT("Player not exist..."));
		return;
	}

	UpdateCurrentMoney(Player->GetItemStatusComponent()->GetCurrentMoney());
	Player->GetItemStatusComponent()->OnMoneyInfoChange.AddUObject(this, &UZShopWidget::UpdateCurrentMoney);

}

void UZShopWidget::OnDrawScreen()
{
	Super::OnDrawScreen();
}

void UZShopWidget::OnRemoveScreen()
{
	Super::OnRemoveScreen();

	if (!bCanDestruct)
	{
		ShopBuyWidget->ClearWidget();
		//ShopSellWidget->ClearWidget();
		//BindShop(nullptr);
	}
	else
	{
		RemoveFromParent();
	}
}

void UZShopWidget::BindShop(AZShop * NewShop)
{
	ZLOG_S(Warning);
	//Shop = NewShop;
	
}

void UZShopWidget::ConstructBuyWidget(UDataTable * const ShopItemDataTable)
{
	TArray<FName> Names = ShopItemDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto ShopItemData = ShopItemDataTable->FindRow<FZShopItemData>(Name, TEXT(""));
		ZLOG(Warning, TEXT("%s"), *ShopItemData->ItemName);

		AddItemToBuyWidget(ShopItemData);
	}	

}

void UZShopWidget::ConstructSellWidget(const TArray<class AZItem*>& ItemList)
{
	for (const auto& Item : ItemList)
	{
		if (nullptr != Item)
		{
			AddItemToSellWidget(Item);
		}
	}

}

void UZShopWidget::AddItemToBuyWidget(FZShopItemData * const NewShopItemData)
{
	ShopBuyWidget->AddItem(NewShopItemData);
	//auto ShopItemWidget = ShopBuyWidget->AddItem(NewShopItemData);
	//if (ShopItemWidget)
	//{
	//	ShopItemWidget->OnBuyShopItem.BindUObject(Shop, &AZShop::Buy);
	//}
}

void UZShopWidget::AddItemToSellWidget(AZItem* NewItem)
{
	/*
		데이터 유효성 검사
	*/
	if (nullptr == NewItem)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}
	
	/*
		SellWidget에 아이템 추가
	*/
	if (nullptr == ShopSellWidget)
	{
		ZLOG(Error, TEXT("Sell widget null."));
		return;
	}
	ShopSellWidget->AddItem(NewItem);
	
	//ShopSellItemWidget->OnSellItem.BindUObject(Shop, &AZShop::Sell);

}

void UZShopWidget::UpdateCurrentMoney(int32 NewMoney)
{
	CurrentMoney->SetText(FText::FromString(FString::FromInt(NewMoney)));
}



void UZShopWidget::OnExitButtonClicked()
{
	//if (Shop)
	//{
	//	ShopBuyWidget->ClearWidget();
	//	ShopSellWidget->ClearWidget();
	//	BindShop(nullptr);
	//}
	//else
	//{
	//	SetVisibility(ESlateVisibility::Hidden);
	//	RemoveFromParent();
	//}
}
