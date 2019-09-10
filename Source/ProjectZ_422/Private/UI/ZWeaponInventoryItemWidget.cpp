// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeaponInventoryItemWidget.h"
#include "ZWeapon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UZWeaponInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewWeaponImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Weapon")));
	check(NewWeaponImage);
	WeaponImage = NewWeaponImage;

}

void UZWeaponInventoryItemWidget::BindWeapon(AZWeapon * NewWeapon)
{
	BindingWeapon = NewWeapon;
	
	if (BindingWeapon.IsValid())
	{
		BindingWeapon.Get()->OnItemRemoved.AddUObject(this, &UZWeaponInventoryItemWidget::ClearWidget);
	}

	UpdateWidget();

}

void UZWeaponInventoryItemWidget::ClearWidget()
{
	if (WeaponImage)
	{
		WeaponImage->SetBrushFromTexture(nullptr);
		WeaponImage->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UZWeaponInventoryItemWidget::UpdateWidget()
{
	if (!BindingWeapon.IsValid())
	{
		return;
	}

	if (WeaponImage)
	{
		WeaponImage->SetBrushFromTexture(BindingWeapon.Get()->GetItemImage());
		WeaponImage->SetVisibility(ESlateVisibility::Visible);
	}

}
