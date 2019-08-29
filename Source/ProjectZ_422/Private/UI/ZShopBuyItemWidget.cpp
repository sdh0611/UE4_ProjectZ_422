// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopBuyItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UZShopBuyItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Item")));
	check(nullptr != NewItemImage);
	ItemImage = NewItemImage;

	auto NewItemName= Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Name")));
	check(nullptr != NewItemName);
	ItemName = NewItemName;

	auto NewItemPrice = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Price")));
	check(nullptr != NewItemPrice);
	ItemPrice = NewItemPrice;

	auto NewBuyButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Buy")));
	check(nullptr != NewBuyButton);
	BuyButton = NewBuyButton;
	BuyButton->OnClicked.AddDynamic(this, &UZShopBuyItemWidget::OnBuyButtonClick);

}

void UZShopBuyItemWidget::BindShopItemData(FZShopItemData * NewShopItemData)
{
	if (nullptr == NewShopItemData)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	ShopItemData = NewShopItemData;
	//ZLOG(Error, TEXT("Item : %s"), *(ShopItemData->ItemName));

	if (nullptr == ItemName)
	{
		ZLOG(Error, TEXT("ItemNameText invalid."));
	}
	else
	{
		ItemName->SetText(FText::FromString(ShopItemData->ItemName));
	}

	if (nullptr == ItemPrice)
	{
		ZLOG(Error, TEXT("ItemPriceText invalid."));
	}
	else 
	{
		ItemPrice->SetText(FText::FromString(FString::FromInt(ShopItemData->ItemPrice).Append("$")));
	}

}

void UZShopBuyItemWidget::OnBuyButtonClick()
{
	ZLOG_S(Warning);

	/*
		일단 임시로 1개씩 차감하도록 설정.
		추후에 판매 개수를 입력받을 수 있게 추가할 예정
	*/
	OnBuyShopItem.Execute(ShopItemData, 1);
}
