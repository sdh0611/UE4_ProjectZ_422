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

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Item")));
	check(ItemImage);
	//ItemImage->SetBrushFromMaterial(Material);

	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Name")));
	check(ItemName);

	ItemQuantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Quantity")));
	check(ItemQuantity);

	ItemPrice = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Price")));
	check(ItemPrice);

	SellButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Buy")));
	check(SellButton);
	SellButton->OnClicked.AddDynamic(this, &UZShopSellItemWidget::OnSellButtonClick);

	SetPadding(5.f);
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

	//ClearWidget();
	//OnSellItem.Execute(GetOwningPlayerPawn(), Item, NewNumber);
}

void UZShopSellItemWidget::BindItem(AZItem * NewItem)
{
	/* nullptr가 들어오면 Widget clear. */
	if (nullptr == NewItem)
	{
		ClearWidget();
		return;
	}

	Item = NewItem;
	/*
		Widget property들 update
	*/
	/*	델리게이트 바인딩 */
	Item->OnItemInfoChanged.AddUObject(this, &UZShopSellItemWidget::UpdateWidget);
	Item->OnItemRemoved.AddUObject(this, &UZShopSellItemWidget::ClearWidget);

	/*	Update item image */
	if (ItemImage)
	{
		UTexture2D* Image = GetGameInstance<UZGameInstance>()->GetItemImage(Item->GetItemName());
		if (Image)
		{
			ItemImage->SetBrushFromTexture(Image);
		}
	}
	else
	{
		ZLOG_S(Error);
	}

	/*	Update item name */
	if (ItemName)
	{
		ItemName->SetText(FText::FromString(Item->GetItemName()));
	}

	/*	Update item quantity */
	if (ItemQuantity)
	{
		FString Quantity = FString::FromInt(Item->GetCurrentQuantityOfItem());
		ItemQuantity->SetText(FText::FromString(Quantity));
	}

	/*	Update item price */
	if (ItemPrice)
	{
		auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
		check(ZGameInstance);

		auto ShopItemData = ZGameInstance->GetShopItemDataByName(NewItem->GetItemName());
		if (nullptr == ShopItemData)
		{
			ZLOG(Error, TEXT("Invalid value : %s"), *NewItem->GetItemName());
			return;
		}
		
		FString Price = FString::FromInt(ShopItemData->ItemPrice * 0.7f).Append("$");
		ItemPrice->SetText(FText::FromString(Price));
	}

	bIsEmpty = false;
	SetVisibility(ESlateVisibility::Visible);
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

	bIsEmpty = true;
	SetVisibility(ESlateVisibility::Collapsed);
	//RemoveFromParent();
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
