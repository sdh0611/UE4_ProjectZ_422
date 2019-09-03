// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCurrentWeaponInfoWidget.h"
#include "ZWeapon.h"
#include "ZGun.h"
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

	auto NewFireMode = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_FireMode")));
	check(nullptr != NewFireMode);
	FireMode = NewFireMode;

	WeaponName->SetText(FText::GetEmpty());
	CurrentAmmo->SetText(FText::GetEmpty());
	MaxAmmo->SetText(FText::GetEmpty());
	FireMode->SetText(FText::GetEmpty());
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

	switch (NewWeapon->GetWeaponCategory())
	{
		case EWeaponCategory::Gun:
		{
			auto Gun = Cast<AZGun>(NewWeapon);

			CurrentAmmo->SetText(FText::FromString(FString::FromInt(Gun->GetCurrentAmmo())));
			MaxAmmo->SetText(FText::FromString(FString::FromInt(Gun->GetMaxAmmo())));
			
			switch (Gun->GetFireMode())
			{
				case EFireMode::AutoFire:
				{
					FireMode->SetText(FText::FromString(TEXT("연사")));
					break;
				}
				case EFireMode::SingleShot:
				{
					FireMode->SetText(FText::FromString(TEXT("단발")));
					break;
				}
				default:
				{
					ZLOG(Error, TEXT("Invalid fire mode value."));
					break;
				}
			}

			break;
		}
		case EWeaponCategory::Grenade:
		{
			FText Text = FText::FromString(FString::FromInt(NewWeapon->GetCurrentQuantityOfItem()));
			CurrentAmmo->SetText(Text);
			MaxAmmo->SetText(Text);
			FireMode->SetText(FText::GetEmpty());
			break;
		}
		case EWeaponCategory::Knife:
		{
			CurrentAmmo->SetText(FText::GetEmpty());
			MaxAmmo->SetText(FText::GetEmpty());
			FireMode->SetText(FText::GetEmpty());
			break;
		}
		default:
		{
			ZLOG(Error, TEXT("Invalid category."));
			break;
		}
	}

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

	switch (Weapon->GetWeaponCategory())
	{
		case EWeaponCategory::Gun:
		{
			auto Gun = Cast<AZGun>(Weapon);
			CurrentAmmo->SetText(FText::FromString(FString::FromInt(Gun->GetCurrentAmmo())));
			
			switch (Gun->GetFireMode())
			{
				case EFireMode::AutoFire:
				{
					FireMode->SetText(FText::FromString(TEXT("연사")));
					break;
				}
				case EFireMode::SingleShot:
				{
					FireMode->SetText(FText::FromString(TEXT("단발")));
					break;
				}
				default:
				{
					ZLOG(Error, TEXT("Invalid fire mode value."));
					break;
				}
			}
			break;
		}
		case EWeaponCategory::Grenade:
		{
			FText Text = FText::FromString(FString::FromInt(Weapon->GetCurrentQuantityOfItem()));
			CurrentAmmo->SetText(Text);
			MaxAmmo->SetText(Text);
			break;
		}
		case EWeaponCategory::Knife:
		{
			break;
		}
		default:
		{
			break;
		}
	}

}

void UZCurrentWeaponInfoWidget::ClearWidget()
{
	Weapon = nullptr;

	WeaponName->SetText(FText::GetEmpty());
	CurrentAmmo->SetText(FText::GetEmpty());
	MaxAmmo->SetText(FText::GetEmpty());
}
