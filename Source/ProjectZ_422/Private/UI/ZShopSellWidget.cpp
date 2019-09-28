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
	WeaponHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("WeaponItemHolder")));
	check(WeaponHolder);

	RecoveryHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("RecoveryItemHolder")));
	check(RecoveryHolder);

	DopingHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("DopingItemHolder")));
	check(DopingHolder);

	AmmoHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("AmmoItemHolder")));
	check(AmmoHolder);

	if (SellItemWidgetClass)
	{
		auto MyPC = GetOwningPlayer();
		/* Weapon holder */
		for (int32 i = 0; i < HolderSize; ++i)
		{
			auto SellItem = CreateWidget<UZShopSellItemWidget>(MyPC, SellItemWidgetClass);
			if (SellItem)
			{
				WeaponHolder->AddChild(SellItem);
				SellItem->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		/* Recovery Holder */
		for (int32 i = 0; i < HolderSize; ++i)
		{
			auto SellItem = CreateWidget<UZShopSellItemWidget>(MyPC, SellItemWidgetClass);
			if (SellItem)
			{
				RecoveryHolder->AddChild(SellItem);
				SellItem->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		/* Doping Holder */
		for (int32 i = 0; i < HolderSize; ++i)
		{
			auto SellItem = CreateWidget<UZShopSellItemWidget>(MyPC, SellItemWidgetClass);
			if (SellItem)
			{
				DopingHolder->AddChild(SellItem);
				SellItem->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		/* Ammo Holder */
		for (int32 i = 0; i < HolderSize; ++i)
		{
			auto SellItem = CreateWidget<UZShopSellItemWidget>(MyPC, SellItemWidgetClass);
			if (SellItem)
			{
				AmmoHolder->AddChild(SellItem);
				SellItem->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

	}

}

void UZShopSellWidget::AddItem(AZItem* NewItem)
{
	ZLOG(Error, TEXT("AddItem to sell widget."));
	switch (NewItem->GetItemType())
	{
		case EItemType::Weapon:
		{
			ZLOG(Error, TEXT("Sell Weapon."));
			auto Children = WeaponHolder->GetAllChildren();
			for (const auto& Child : Children)
			{
				auto SellItem = Cast<UZShopSellItemWidget>(Child);
				if (SellItem && SellItem->bIsEmpty)
				{
					SellItem->BindItem(NewItem);
					return;
				}
			}
			break;
		}
		case EItemType::Recovery:
		{
			ZLOG(Error, TEXT("Sell Recovery."));
				auto Children = RecoveryHolder->GetAllChildren();
			for (const auto& Child : Children)
			{
				auto SellItem = Cast<UZShopSellItemWidget>(Child);
				if (SellItem && SellItem->bIsEmpty)
				{
					SellItem->BindItem(NewItem);
					return;
				}
			}
			break;
		}
		case EItemType::Doping:
		{
			ZLOG(Error, TEXT("Sell Doping."));
			auto Children = DopingHolder->GetAllChildren();
			for (const auto& Child : Children)
			{
				auto SellItem = Cast<UZShopSellItemWidget>(Child);
				if (SellItem && SellItem->bIsEmpty)
				{
					SellItem->BindItem(NewItem);
					return;
				}
			}
			break;
		}
		case EItemType::Ammo:
		{
			ZLOG(Error, TEXT("Sell Ammo."));
			auto Children = AmmoHolder->GetAllChildren();
			for (const auto& Child : Children)
			{
				auto SellItem = Cast<UZShopSellItemWidget>(Child);
				if (SellItem && SellItem->bIsEmpty)
				{
					SellItem->BindItem(NewItem);
					return;
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}

}

void UZShopSellWidget::ClearWidget()
{
	WeaponHolder->ClearChildren();
	RecoveryHolder->ClearChildren();
	DopingHolder->ClearChildren();
	AmmoHolder->ClearChildren();
}
