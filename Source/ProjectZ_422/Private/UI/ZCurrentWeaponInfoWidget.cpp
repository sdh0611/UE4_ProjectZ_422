// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCurrentWeaponInfoWidget.h"
#include "ZWeapon.h"
#include "ZGun.h"
#include "ZGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


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

	auto NewWeaponIcon = Cast<UImage>(GetWidgetFromName(TEXT("IMG_WeaponIcon")));
	check(nullptr != NewWeaponIcon);
	WeaponIcon = NewWeaponIcon;

	WeaponName->SetText(FText::GetEmpty());
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(0)));
	MaxAmmo->SetText(FText::FromString(FString::FromInt(0)));
	FireMode->SetText(FText::GetEmpty());
	WeaponIcon->SetVisibility(ESlateVisibility::Hidden);

	LoadIconImage();

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

	WeaponIcon->SetBrushFromTexture(FindTexture(*NewWeapon->GetItemName()));
	WeaponIcon->SetVisibility(ESlateVisibility::Visible);

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
			CurrentAmmo->SetText(FText::FromString(FString::FromInt(0)));
			MaxAmmo->SetText(FText::FromString(FString::FromInt(0)));

			FireMode->SetText(FText::GetEmpty());
			break;
		}
		default:
		{
			ZLOG(Error, TEXT("Invalid category."));

			WeaponIcon->SetVisibility(ESlateVisibility::Hidden);
			WeaponIcon->SetBrushFromTexture(nullptr);
			
			CurrentAmmo->SetText(FText::GetEmpty());

			MaxAmmo->SetText(FText::GetEmpty());
			
			FireMode->SetText(FText::GetEmpty());
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

	WeaponIcon->SetVisibility(ESlateVisibility::Hidden);
	WeaponIcon->SetBrushFromTexture(nullptr);

	WeaponName->SetText(FText::GetEmpty());
	
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(0)));
	
	MaxAmmo->SetText(FText::FromString(FString::FromInt(0)));
	
	FireMode->SetText(FText::GetEmpty());
}

UTexture2D * const UZCurrentWeaponInfoWidget::FindTexture(const FName & Name) const
{
	if (IconTable.Contains(Name))
	{
		return IconTable[Name];
	}

	return nullptr;
}

void UZCurrentWeaponInfoWidget::LoadIconImage()
{
	if (nullptr == IconDataTable)
	{
		ZLOG(Error, TEXT("IconData not exist.."));
		return;
	}

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(MyGameInstance);

	//TArray<FName> Names = IconDataTable->GetRowNames();

	//for (const auto& Name : Names)
	//{
	//	auto Data = IconDataTable->FindRow<FZImageData>(Name, TEXT(""));
	//	if (Data)
	//	{
	//		FSoftObjectPath Path(Data->ImagePath);
	//		MyGameInstance->AssetLoader.RequestSyncLoad(Path);

	//		TSoftObjectPtr<UTexture2D> Icon(Data->ImagePath);
	//		if (Icon.IsValid())
	//		{
	//			IconTable.Add(*Data->Name, Icon.Get());
	//		}

	//	}
	//	
	//}

	TArray<FName> Names = IconDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = IconDataTable->FindRow<FZImageData>(Name, TEXT(""));

		if (!Data->ImagePath.IsNull())
		{
			IconTable.Add(*Data->Name, MyGameInstance->AssetLoader.LoadSynchronous(Data->ImagePath));
		}

	}



}
