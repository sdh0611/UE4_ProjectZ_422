// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopSellItemWidget.h"
#include "ZItem.h"
#include "ZWeapon.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZInputNumberWidget.h"
#include "ZGameInstance.h"
#include "ZPlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Materials/Material.h"

void UZShopSellItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Item")));
	check(nullptr != NewItemImage);
	ItemImage = NewItemImage;
	//ItemImage->SetBrushFromMaterial(Material);

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

void UZShopSellItemWidget::OnReceiveNumberInput(int32 NewNumber)
{
	if (NewNumber < 1)
	{
		return;
	}

	auto MyPC = GetOwningPlayer<AZPlayerController>();
	if (MyPC)
	{
		MyPC->Sell(Item->GetInventoryIndex(), NewNumber);
	}
	//OnSellItem.Execute(GetOwningPlayerPawn(), Item, NewNumber);
}

void UZShopSellItemWidget::BindItem(AZItem * NewItem)
{
	Item = NewItem;

	/*
		Widget property들 update
	*/
	if (nullptr == Item)
	{
		ZLOG(Error, TEXT("Item is null"));
		return;
	}

	/*
		델리게이트 바인딩
	*/
	Item->OnItemRemoved.AddUObject(this, &UZShopSellItemWidget::ClearWidget);

	/*
		Update item image
	*/
	ItemImage->SetBrushFromTexture(Item->GetItemImage());

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
	if (!Item.IsValid())
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
	
	if (Item->GetCurrentQuantityOfItem() <= 1)
	{
		OnReceiveNumberInput(1);
	}
	else
	{
		auto PC = GetOwningPlayer<AZPlayerController>();
		if (PC)
		{
			auto UserHUD = PC->GetUserHUD();
			if (UserHUD)
			{
				auto InputWidget = UserHUD->GetInputNumberWidget();
				if (InputWidget)
				{
					InputWidget->SetVisibility(ESlateVisibility::Visible);
					InputWidget->BindWidget(this);
				}
			}

		}
	}

}
