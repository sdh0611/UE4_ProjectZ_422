// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInventoryItemWidget.h"
#include "ZItem.h"
#include "ZPlayerController.h"
#include "ZUsableItemInterface.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZInputNumberWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


UZInventoryItemWidget::UZInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	Item = nullptr;
}

void UZInventoryItemWidget::NativeConstruct()
{
	ZLOG(Warning, TEXT("ItemWidget construct."));
	Super::NativeConstruct();

	auto NewItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_ItemImage")));
	check(nullptr != NewItemImage);
	ItemImage = NewItemImage;

	auto NewItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_ItemName")));
	check(nullptr != NewItemName);
	ItemName = NewItemName;

	auto NewQuantityOfItem = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_QuantityOfItem")));
	check(nullptr != NewQuantityOfItem);
	QuantityOfItem = NewQuantityOfItem;

	auto NewDropButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Drop")));
	check(nullptr != NewDropButton);
	DropButton = NewDropButton;
	DropButton->OnClicked.AddDynamic(this, &UZInventoryItemWidget::OnDropButtonClicked);
	
}

void UZInventoryItemWidget::OnReceiveNumberInput(int32 NewNumber)
{
	if (NewNumber < 1)
	{
		return;
	}

	Item->OnDropped(NewNumber);
}

void UZInventoryItemWidget::BindItem(AZItem * NewItem)
{
	if (nullptr == NewItem)
	{
		ZLOG(Warning, TEXT("Invalid item pointer."));
		return;
	}

	ZLOG(Warning, TEXT("Bind item pointer."));

	Item = NewItem;

	ItemName->SetText(FText::FromString(Item->GetItemName()));

	FString Quantity = FString::FromInt(Item->GetCurrentQuantityOfItem());
	QuantityOfItem->SetText(FText::FromString(Quantity));

	ItemImage->SetBrushFromTexture(Item->GetItemImage());

	// Item 정보 갱신을 위한 델리게이트 바인딩.
	NewItem->OnItemInfoChanged.AddUObject(this, &UZInventoryItemWidget::UpdateWidget);	
	// Item 목록에서 삭제하기 위한 델리게이트 바인딩.
	NewItem->OnItemRemoved.AddUObject(this, &UZInventoryItemWidget::ClearWidget);
}

AZItem * const UZInventoryItemWidget::GetBindingItem() const
{
	return Item;
}

FReply UZInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	auto Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return Result;
	}

	ZLOG_S(Warning);

	if (nullptr == Item)
	{
		return Result;
	}

	auto UsableItem = Cast<IZUsableItemInterface>(Item);
	if (UsableItem)
	{
		UsableItem->OnUsed();
	}

	return Result;
}


void UZInventoryItemWidget::ClearWidget()
{
	ZLOG_S(Warning);
	// Delegate 해제
	Item->OnItemInfoChanged.RemoveAll(this);
	Item->OnItemRemoved.RemoveAll(this);

	Item = nullptr;

	RemoveFromParent();
}

void UZInventoryItemWidget::UpdateWidget()
{
	if (nullptr == Item)
	{
		ZLOG(Error, TEXT("Item is null"));
		return;
	}
	ZLOG_S(Warning);

	ItemName->SetText(FText::FromString(Item->GetItemName()));

	FString Quantity = FString::FromInt(Item->GetCurrentQuantityOfItem());
	QuantityOfItem->SetText(FText::FromString(Quantity));

}

// 폐기예정(5.18)
void UZInventoryItemWidget::OnDropButtonClicked()
{

	if (Item->GetCurrentQuantityOfItem() <= 1)
	{
		OnReceiveNumberInput(1);
	}
	else
	{
		auto PC = GetOwningPlayer<AZPlayerController>();
		if (PC)
		{
			auto UserHUD = PC->GetZHUD()->GetUserHUD();
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
