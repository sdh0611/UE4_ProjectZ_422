// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopWidget.h"
#include "ZShop.h"
#include "ZShopBuyWidget.h"
#include "ZShopBuyItemWidget.h"
#include "ZShopSellWidget.h"
#include "ZShopSellItemWidget.h"
#include "ZItem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"


void UZShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewExitButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Exit")));
	check(nullptr != NewExitButton);
	ExitButton = NewExitButton;

	SetVisibility(ESlateVisibility::Hidden);
	ExitButton->OnClicked.AddDynamic(this, &UZShopWidget::OnExitButtonClicked);

	auto NewShopBuyWidget = Cast<UZShopBuyWidget>(GetWidgetFromName(TEXT("UI_Shop_Buy")));
	check(nullptr != NewShopBuyWidget);
	ShopBuyWidget = NewShopBuyWidget;

	auto NewShopSellWidget = Cast<UZShopSellWidget>(GetWidgetFromName(TEXT("UI_Shop_Sell")));
	check(nullptr != NewShopSellWidget);
	ShopSellWidget = NewShopSellWidget;

}

void UZShopWidget::BindShop(AZShop * NewShop)
{
	ZLOG_S(Warning);
	Shop = NewShop;
	
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
	auto ShopItemWidget = ShopBuyWidget->AddItem(NewShopItemData);
	if (ShopItemWidget)
	{
		ShopItemWidget->OnBuyShopItem.BindUObject(Shop, &AZShop::Buy);
	}
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
	auto ShopSellItemWidget = ShopSellWidget->AddItem(NewItem);
	if (nullptr == ShopSellItemWidget)
	{
		ZLOG(Error, TEXT("Invalid widget."));
		return;
	}
	
	ShopSellItemWidget->OnSellItem.BindUObject(Shop, &AZShop::Sell);

}



void UZShopWidget::OnExitButtonClicked()
{
	if (Shop)
	{
		Shop->OnExitShop();
		ShopBuyWidget->ClearWidget();
		ShopSellWidget->ClearWidget();
	}
	else
	{
		RemoveFromParent();
	}
}
