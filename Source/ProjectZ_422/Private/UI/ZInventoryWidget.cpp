// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInventoryWidget.h"
#include "ZInventoryItemWidget.h"
#include "ZItem.h"
#include "ZWeapon.h"
#include "Components/ScrollBox.h"
#include "ConstructorHelpers.h"

UZInventoryWidget::UZInventoryWidget(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	//static ConstructorHelpers::FClassFinder<UZInventoryItemWidget>
	//	UI_ITEM(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/UI_InventoryItem.UI_InventoryItem_C'"));
	//if (UI_ITEM.Succeeded())
	//{
	//	InventoryItemWidgetClass = UI_ITEM.Class;
	//}

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
	ZLOG_S(Warning);
	auto Widget = CreateWidget<UZInventoryItemWidget>(GetOwningPlayer(), InventoryItemWidgetClass);
	if (Widget)
	{
		Widget->SetPadding(5.f);
		ItemHolder->AddChild(Widget);
		Widget->BindItem(NewItem);
	}
	else
	{
		ZLOG(Warning, TEXT("Widget pool not available!"));
	}


}

void UZInventoryWidget::AddItemToWeaponInventory(AZWeapon * const NewWeapon)
{
}
