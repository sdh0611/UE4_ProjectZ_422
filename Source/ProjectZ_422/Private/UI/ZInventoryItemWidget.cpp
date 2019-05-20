// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInventoryItemWidget.h"
#include "ZItem.h"
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

	// Item ���� ������ ���� ��������Ʈ ���ε�.
	NewItem->OnItemInfoChanged.AddUObject(this, &UZInventoryItemWidget::UpdateWidget);	
	// Item ��Ͽ��� �����ϱ� ���� ��������Ʈ ���ε�.
	NewItem->OnItemRemoved.AddUObject(this, &UZInventoryItemWidget::ClearWidget);
}

AZItem * const UZInventoryItemWidget::GetBindingItem() const
{
	return Item;
}


void UZInventoryItemWidget::ClearWidget()
{
	ZLOG_S(Warning);
	// Delegate ����
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

// ��⿹��(5.18)
void UZInventoryItemWidget::OnDropButtonClicked()
{
	Item->OnDropped();
	//ClearWidget();
}
