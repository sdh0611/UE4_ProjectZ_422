// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopSellItemWidget.h"
#include "ZItem.h"
#include "ZGameInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void UZShopSellItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Item")));
	check(nullptr != NewItemImage);
	ItemImage = NewItemImage;

	auto NewItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Name")));
	check(nullptr != NewItemName);
	ItemName = NewItemName;

	auto NewItemQuantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Quantity")));
	check(nullptr != NewItemQuantity);
	ItemQuantity = NewItemQuantity;

	auto NewItemPrice = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Price")));
	check(nullptr != NewItemPrice);
	ItemPrice = NewItemPrice;

	auto NewSellButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Buy")));
	check(nullptr != NewSellButton);
	SellButton = NewSellButton;
	SellButton->OnClicked.AddDynamic(this, &UZShopSellItemWidget::OnSellButtonClick);
}

void UZShopSellItemWidget::BindItem(AZItem * NewItem)
{
	Item = NewItem;

	/*
		Widget property�� update
	*/
	if (nullptr == Item)
	{
		ZLOG(Error, TEXT("Item is null"));
		return;
	}

	/*
		��������Ʈ ���ε�
	*/
	Item->OnItemRemoved.AddUObject(this, &UZShopSellItemWidget::ClearWidget);
	Item->OnItemInfoChanged.AddUObject(this, &UZShopSellItemWidget::UpdateWidget);

	/*
		Update item name
	*/
	ItemName->SetText(FText::FromString(Item->GetItemName()));

	/*
		Update item quantity
	*/
	FString Quantity = FString::FromInt(Item->GetCurrentQuantityOfItem());
	ItemQuantity->SetText(FText::FromString(Quantity));

	/*
		Update item price
	*/
	auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(nullptr != ZGameInstance);
	auto ShopItemData = ZGameInstance->GetShopItemDataByName(NewItem->GetItemName());
	if (nullptr == ShopItemData)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}
	FString Price = FString::FromInt(ShopItemData->ItemPrice * 0.7f).Append("$");
	ItemPrice->SetText(FText::FromString(Price));
	
}

void UZShopSellItemWidget::UpdateWidget()
{
	if (nullptr == Item)
	{
		ZLOG(Error, TEXT("Item is null"));
		return;
	}

	FString Quantity = FString::FromInt(Item->GetCurrentQuantityOfItem());
	ItemQuantity->SetText(FText::FromString(Quantity));
}

void UZShopSellItemWidget::ClearWidget()
{
	Item = nullptr;

	RemoveFromParent();
}

void UZShopSellItemWidget::OnSellButtonClick()
{
	ZLOG_S(Warning);
	/*
		�ϴ� �ӽ÷� 1���� �����ϵ��� ����.
		���Ŀ� �Ǹ� ������ �Է¹��� �� �ְ� �߰��� ����
	*/
	OnSellItem.Execute(Item, 1);
}
