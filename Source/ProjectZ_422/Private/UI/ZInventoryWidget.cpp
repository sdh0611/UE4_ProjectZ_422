// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInventoryWidget.h"
#include "ZInventoryItemWidget.h"
#include "ZWeaponInventoryItemWidget.h"
#include "ZItem.h"
#include "ZWeapon.h"
#include "ZWeaponInventoryWidget.h"
#include "Components/ScrollBox.h"
#include "ConstructorHelpers.h"

UZInventoryWidget::UZInventoryWidget(const FObjectInitializer& ObjectInitializer)
	:UZUserWidget(ObjectInitializer)
{
}

void UZInventoryWidget::NativeConstruct()
{
	ZLOG(Warning, TEXT("InventoryWidget construct."));
	Super::NativeConstruct();

	ItemHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemHolder")));
	check(ItemHolder);

	WeaponInventoryWidget = Cast <UZWeaponInventoryWidget>(GetWidgetFromName(TEXT("UI_WeaponInventory")));
	check(WeaponInventoryWidget);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UZInventoryWidget::AddItemToInventoryWidget(AZItem * const NewItem)
{
	if (::IsValid(NewItem))
	{
		if (EItemType::Weapon == NewItem->GetItemType())
		{
			auto Weapon = Cast<AZWeapon>(NewItem);
			if (::IsValid(Weapon))
			{
				if (EWeaponCategory::Grenade == Weapon->GetWeaponCategory())
				{
					AddItemToInventory(NewItem);
				}
				else
				{
					AddItemToWeaponInventory(Weapon);
				}
			}
		}
		else
		{
			AddItemToInventory(NewItem);
		}
	}

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
	//auto WeaponWidget = Cast<UZWeaponInventoryItemWidget>(WeaponHolder->GetChildAt(NewWeapon->GetWeaponInventoryIndex()));
	//if (nullptr == WeaponWidget)
	//{
	//	ZLOG(Error, TEXT("Weapon Inventory widget not exist."));
	//	return;
	//}
	if (WeaponInventoryWidget)
	{
		WeaponInventoryWidget->AddItemToWeaponInventory(NewWeapon);
	}
}
