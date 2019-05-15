// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInventoryWidget.h"
#include "ZInventoryItemWidget.h"
#include "ZItem.h"
#include "Components/ScrollBox.h"
#include "ConstructorHelpers.h"

UZInventoryWidget::UZInventoryWidget(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UZInventoryItemWidget>
		UI_ITEM(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/UI_InventoryItem.UI_InventoryItem_C'"));
	if (UI_ITEM.Succeeded())
	{
		InventoryItemWidgetClass = UI_ITEM.Class;
	}

}

void UZInventoryWidget::NativeConstruct()
{
	ZLOG(Warning, TEXT("InventoryWidget construct."));
	Super::NativeConstruct();

	auto NewItemHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemHolder")));
	check(nullptr != NewItemHolder);
	ItemHolder = NewItemHolder;

	SetVisibility(ESlateVisibility::Hidden);
}

void UZInventoryWidget::AddItemToInventory(AZItem * const NewItem)
{
	auto NewInventoryItemWidget = CreateWidget<UZInventoryItemWidget>(GetOwningPlayer(), InventoryItemWidgetClass);
	if (NewInventoryItemWidget)
	{
		ItemHolder->AddChild(NewInventoryItemWidget);
		NewInventoryItemWidget->BindItem(NewItem);
	}

}
