// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInventoryItemWidget.h"
#include "ZItem.h"
#include "ZPlayerController.h"
#include "ZUsableItemInterface.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZInputNumberWidget.h"
#include "ZGameInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"


UZInventoryItemWidget::UZInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	Item = nullptr;
}

void UZInventoryItemWidget::NativeConstruct()
{
	ZLOG(Warning, TEXT("ItemWidget construct."));
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_ItemImage")));
	check(ItemImage);

	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_ItemName")));
	check(ItemName);

	QuantityOfItem = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_QuantityOfItem")));
	check(QuantityOfItem);

	DropButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Drop")));
	check(DropButton);
	DropButton->OnClicked.AddDynamic(this, &UZInventoryItemWidget::OnDropButtonClicked);

}

void UZInventoryItemWidget::OnReceiveNumberInput(int32 NewNumber)
{
	if (NewNumber < 1)
	{
		return;
	}

	if (Item.IsValid())
	{
		Item->OnDropped(NewNumber);
	}
}

void UZInventoryItemWidget::BindItem(AZItem * NewItem)
{
	if (nullptr == NewItem)
	{
		ClearWidget();
		return;
	}

	Item = NewItem;

	bIsEmpty = false;

	if (ItemName)
	{
		ItemName->SetText(FText::FromString(Item->GetItemName()));
	}

	if (QuantityOfItem)
	{
		FString Quantity = FString::FromInt(Item->GetCurrentQuantityOfItem());
		QuantityOfItem->SetText(FText::FromString(Quantity));
	}

	if (ItemImage)
	{
		UTexture2D* Image = GetGameInstance<UZGameInstance>()->GetItemImage(Item->GetItemName());
		ItemImage->SetBrushFromTexture(Image);
	}

	// Item ���� ������ ���� ��������Ʈ ���ε�.
	NewItem->OnItemInfoChanged.AddUObject(this, &UZInventoryItemWidget::UpdateWidget);
	// Item ��Ͽ��� �����ϱ� ���� ��������Ʈ ���ε�.
	NewItem->OnItemRemoved.AddUObject(this, &UZInventoryItemWidget::ClearWidget);
}

AZItem * const UZInventoryItemWidget::GetBindingItem() const
{
	return Item.Get();
}

FReply UZInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	auto Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return Result;
	}

	if (!Item.IsValid())
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
	Item = nullptr;
	
	ZLOG_S(Error);
	bIsEmpty = true;

	RemoveFromParent();
}

void UZInventoryItemWidget::UpdateWidget()
{
	if (!Item.IsValid())
	{
		ZLOG(Error, TEXT("Item is invalid"));
		return;
	}
	ZLOG_S(Warning);

	ItemName->SetText(FText::FromString(Item->GetItemName()));

	FString Quantity = FString::FromInt(Item->GetCurrentQuantityOfItem());
	QuantityOfItem->SetText(FText::FromString(Quantity));

}

// ��⿹��(5.18)
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
