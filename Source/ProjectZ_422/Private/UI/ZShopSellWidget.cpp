// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopSellWidget.h"
#include "ZShopSellItemWidget.h"
#include "ZItem.h"
#include "Components/ScrollBox.h"


void UZShopSellWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*
	   각 Tab별 아이템 목록을 관리하는 ScrollBox
	*/
	auto NewWeaponHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("WeaponItemHolder")));
	check(nullptr != NewWeaponHolder);
	WeaponHolder = NewWeaponHolder;

	auto NewRecoveryHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("RecoveryItemHolder")));
	check(nullptr != NewRecoveryHolder);
	RecoveryHolder = NewRecoveryHolder;

	auto NewDopingHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("DopingItemHolder")));
	check(nullptr != NewDopingHolder);
	DopingHolder = NewDopingHolder;

	auto NewAmmoHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("AmmoItemHolder")));
	check(nullptr != NewAmmoHolder);
	AmmoHolder = NewAmmoHolder;

}

UZShopSellItemWidget* UZShopSellWidget::AddItem(AZItem* NewItem)
{
	check(nullptr != SellItemWidgetClass);
	auto SellItemWidget = CreateWidget<UZShopSellItemWidget>(GetOwningPlayer(), SellItemWidgetClass);
	if (nullptr == SellItemWidget)
	{
		ZLOG(Error, TEXT("Failed to create widget.."));
		return nullptr;
	}

	SellItemWidget->SetPadding(5.f);

	switch (NewItem->GetItemType())
	{
		case EItemType::Weapon:
		{
			WeaponHolder->AddChild(SellItemWidget);
			break;
		}
		case EItemType::Recovery:
		{
			RecoveryHolder->AddChild(SellItemWidget);
			break;
		}
		case EItemType::Doping:
		{
			DopingHolder->AddChild(SellItemWidget);
			break;
		}
		case EItemType::Ammo:
		{
			AmmoHolder->AddChild(SellItemWidget);
			break;
		}
		default:
		{
			break;
		}
	}

	SellItemWidget->BindItem(NewItem);

	return SellItemWidget;
}

void UZShopSellWidget::ClearWidget()
{
	WeaponHolder->ClearChildren();
	RecoveryHolder->ClearChildren();
	DopingHolder->ClearChildren();
	AmmoHolder->ClearChildren();
}
