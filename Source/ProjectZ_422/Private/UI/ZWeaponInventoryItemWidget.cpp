// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeaponInventoryItemWidget.h"
#include "ZWeapon.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZGameInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UZWeaponInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Weapon")));
	check(WeaponImage);

}

void UZWeaponInventoryItemWidget::BindWeapon(AZWeapon * NewWeapon)
{
	ZLOG(Error, TEXT("binding weapon."));
	if (nullptr == NewWeapon)
	{
		ClearWidget();
		return;
	}

	BindingWeapon = NewWeapon;
	ZLOG(Error, TEXT("In binding weapon."));

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
		if (BindingWeapon.IsValid())
		{
			BindingWeapon.Get()->OnItemRemoved.RemoveAll(this);
		}
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
		UTexture2D* Image = GetGameInstance<UZGameInstance>()->GetItemImage(BindingWeapon->GetItemName());
		WeaponImage->SetBrushFromTexture(Image);
		WeaponImage->SetVisibility(ESlateVisibility::Visible);
	}


}
