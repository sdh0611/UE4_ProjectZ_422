// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopBuyWidget.h"
#include "ZShopBuyItemWidget.h"
#include "ZItem.h"
#include "ZTooltipWidget.h"
#include "Components/ScrollBox.h"


void UZShopBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	 /*
		각 Tab별 아이템 목록을 관리하는 ScrollBox
	 */
	auto NewWeaponHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("WeaponHolder")));
	check(nullptr != NewWeaponHolder);
	WeaponHolder = NewWeaponHolder;

	auto NewRecoveryHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("RecoveryHolder")));
	check(nullptr != NewRecoveryHolder);
	RecoveryHolder = NewRecoveryHolder;

	auto NewDopingHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("DopingHolder")));
	check(nullptr != NewDopingHolder);
	DopingHolder = NewDopingHolder;

	auto NewAmmoHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("AmmoHolder")));
	check(nullptr != NewAmmoHolder);
	AmmoHolder = NewAmmoHolder;

	auto NewToolTipWidget = Cast<UZTooltipWidget>(GetWidgetFromName(TEXT("UI_ToolTip")));
	check(NewToolTipWidget);
	ShopToolTipWidget = NewToolTipWidget;

}

UZShopBuyItemWidget* UZShopBuyWidget::AddItem(FZShopItemData* NewItemData)
{
	check(nullptr != BuyItemWidgetClass);
	auto BuyItemWidget = CreateWidget<UZShopBuyItemWidget>(GetOwningPlayer(), BuyItemWidgetClass);
	if (nullptr == BuyItemWidget)
	{
		ZLOG(Error, TEXT("Failed to create widget.."));
		return nullptr;
	}

	BuyItemWidget->SetPadding(5.f);

	switch (GetItemTypeFromString(NewItemData->ItemType))
	{
		case EItemType::Weapon:
		{
			WeaponHolder->AddChild(BuyItemWidget);
			break;
		}
		case EItemType::Recovery:
		{
			RecoveryHolder->AddChild(BuyItemWidget);
			break;
		}
		case EItemType::Doping:
		{
			DopingHolder->AddChild(BuyItemWidget);
			break;
		}
		case EItemType::Ammo:
		{
			AmmoHolder->AddChild(BuyItemWidget);
			break;
		}
		default:
		{
			break;
		}
	}
	
	BuyItemWidget->SetParentShopWidget(this);
	BuyItemWidget->BindShopItemData(NewItemData);

	return BuyItemWidget;
}

void UZShopBuyWidget::ClearWidget()
{
	WeaponHolder->ClearChildren();
	RecoveryHolder->ClearChildren();
	DopingHolder->ClearChildren();
	AmmoHolder->ClearChildren();
}

UZTooltipWidget * const UZShopBuyWidget::GetToolTipWidget() const
{
	return ShopToolTipWidget;
}
