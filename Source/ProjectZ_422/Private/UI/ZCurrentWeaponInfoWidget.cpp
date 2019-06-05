// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCurrentWeaponInfoWidget.h"
#include "ZWeapon.h"
#include "Components/TextBlock.h"

void UZCurrentWeaponInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewWeaponName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_WeaponName")));
	check(nullptr != NewWeaponName);
	WeaponName = NewWeaponName;

	auto NewCurrentAmmo= Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrentAmmo")));
	check(nullptr != NewCurrentAmmo);
	CurrentAmmo = NewCurrentAmmo;

	auto NewMaxAmmo= Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_MaxAmmo")));
	check(nullptr != NewMaxAmmo);
	MaxAmmo = NewMaxAmmo;

	WeaponName->SetText(FText::GetEmpty());
	CurrentAmmo->SetText(FText::GetEmpty());
	MaxAmmo->SetText(FText::GetEmpty());
}

void UZCurrentWeaponInfoWidget::BindWeapon(AZWeapon * NewWeapon)
{
	if (nullptr == NewWeapon)
	{
		ZLOG(Error, TEXT("Invalud value."));
		return;
	}

	Weapon = NewWeapon;

	WeaponName->SetText(FText::FromString(Weapon->GetItemName()));

	CurrentAmmo->SetText(FText::FromString(FString::FromInt(Weapon->GetCurrentAmmo())));

	MaxAmmo->SetText(FText::FromString(FString::FromInt(Weapon->GetMaxAmmo())));

	Weapon->OnItemInfoChanged.AddUObject(this, &UZCurrentWeaponInfoWidget::UpdateWidget);
	Weapon->OnItemRemoved.AddUObject(this, &UZCurrentWeaponInfoWidget::ClearWidget);

}

void UZCurrentWeaponInfoWidget::UpdateWidget()
{
	if (nullptr == Weapon)
	{
		ZLOG(Error, TEXT("Weapon null"));
		return;
	}

	CurrentAmmo->SetText(FText::FromString(FString::FromInt(Weapon->GetCurrentAmmo())));
}

void UZCurrentWeaponInfoWidget::ClearWidget()
{
	Weapon = nullptr;

	WeaponName->SetText(FText::GetEmpty());
	CurrentAmmo->SetText(FText::GetEmpty());
	MaxAmmo->SetText(FText::GetEmpty());
}
