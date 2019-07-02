// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopBuyWidget.h"
#include "ZShopBuyItemWidget.h"
#include "ZItem.h"
#include "Components/ScrollBox.h"


void UZShopBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	 /*
		�� Tab�� ������ ����� �����ϴ� ScrollBox
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
		default:
		{
			break;
		}
	}
	
	BuyItemWidget->BindShopItemData(NewItemData);

	return BuyItemWidget;
}

void UZShopBuyWidget::ClearWidget()
{
	WeaponHolder->ClearChildren();
	RecoveryHolder->ClearChildren();
	DopingHolder->ClearChildren();
}