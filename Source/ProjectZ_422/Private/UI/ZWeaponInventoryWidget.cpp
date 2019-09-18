// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeaponInventoryWidget.h"
#include "ZWeaponInventoryItemWidget.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZGameInstance.h"
#include "ZWeapon.h"
#include "Components/Image.h"

void UZWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponMainFirst = Cast<UZWeaponInventoryItemWidget>(GetWidgetFromName(TEXT("UI_WeaponMain1")));
	check(WeaponMainFirst);

	WeaponMainSecond = Cast<UZWeaponInventoryItemWidget>(GetWidgetFromName(TEXT("UI_WeaponMain2")));
	check(WeaponMainSecond);

	WeaponGrenade = Cast<UZWeaponInventoryItemWidget>(GetWidgetFromName(TEXT("UI_WeaponGrenade")));
	check(WeaponGrenade);

}

void UZWeaponInventoryWidget::AddItemToWeaponInventory(class AZWeapon* NewWeapon)
{


	switch (NewWeapon->GetWeaponInventoryIndex())
	{
		case EWeaponSlot::Main1:
		{
			if (WeaponMainFirst)
			{
				WeaponMainFirst->BindWeapon(NewWeapon);
			}
			break;
		}
		case EWeaponSlot::Main2:
		{
			if (WeaponMainSecond)
			{
				WeaponMainSecond->BindWeapon(NewWeapon);
			}
			break;
		}
		case EWeaponSlot::Grenade:
		{
			if (WeaponGrenade)
			{
				WeaponGrenade->BindWeapon(NewWeapon);
			}
			break;
		}
		default:
		{
			ZLOG(Error, TEXT("Invalid weapon slot.."));
			break;
		}
	}

}

void UZWeaponInventoryWidget::UpdateWidget()
{
}

