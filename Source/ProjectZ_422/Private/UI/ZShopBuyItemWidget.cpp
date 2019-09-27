// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopBuyItemWidget.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZInputNumberWidget.h"
#include "ZGameInstance.h"
#include "ZShopBuyWidget.h"
#include "ZTooltipWidget.h"
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

void UZShopBuyItemWidget::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (ParentShopWidget.IsValid())
	{
		ParentShopWidget.Get()->GetToolTipWidget()->BindItemInfo(ItemName->GetText().ToString());
	}

}

void UZShopBuyItemWidget::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	//if (ParentShopWidget.IsValid())
	//{
	//	ParentShopWidget.Get()->GetToolTipWidget()->ClearWidget();
	//}

}

void UZShopBuyItemWidget::OnReceiveNumberInput(int32 NewNumber)
{
	if (NewNumber < 1)
	{
		return;
	}

	OnBuyShopItem.Execute(GetOwningPlayerPawn(), ShopID, NewNumber);
}

void UZShopBuyItemWidget::SetParentShopWidget(UZShopBuyWidget * NewParent)
{
	ParentShopWidget = NewParent;
}

void UZShopBuyItemWidget::BindShopItemData(FZShopItemData * NewShopItemData)
{
	if (nullptr == NewShopItemData)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	bIsDealOnlyOne = NewShopItemData->bIsDealOnlyOne;
	ShopID = NewShopItemData->ShopID;
	//ShopItemData = NewShopItemData;
	//ZLOG(Error, TEXT("Item : %s"), *(ShopItemData->ItemName));

	if (nullptr == ItemImage)
	{
		ZLOG(Error, TEXT("ItemImage invalid."));
	}
	else
	{
		ItemImage->SetBrushFromTexture(GetGameInstance<UZGameInstance>()->GetItemImage(NewShopItemData->ItemName));
	}


	if (nullptr == ItemName)
	{
		ZLOG(Error, TEXT("ItemNameText invalid."));
	}
	else
	{
		ItemName->SetText(FText::FromString(NewShopItemData->ItemName));
	}

	if (nullptr == ItemPrice)
	{
		ZLOG(Error, TEXT("ItemPriceText invalid."));
	}
	else 
	{
		ItemPrice->SetText(FText::FromString(FString::FromInt(NewShopItemData->ItemPrice).Append("$")));
	}


}


void UZShopBuyItemWidget::OnBuyButtonClick()
{
	ZLOG_S(Warning);

	if (bIsDealOnlyOne)
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
					InputWidget->OnDrawScreen();
					InputWidget->BindWidget(this);
				}
			}

		}
	}

}
