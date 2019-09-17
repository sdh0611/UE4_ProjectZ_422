// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeaponInventoryWidget.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZGameInstance.h"
#include "Components/Image.h"

void UZWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponMainFirst = Cast<UImage>(GetWidgetFromName(TEXT("IMG_WeaponMain1")));
	check(WeaponMainFirst);

	WeaponMainSecond = Cast<UImage>(GetWidgetFromName(TEXT("IMG_WeaponMain2")));
	check(WeaponMainSecond);

	WeaponGrenade = Cast<UImage>(GetWidgetFromName(TEXT("IMG_WeaponGrenade")));
	check(WeaponGrenade);

	LoadIconImage();
}

void UZWeaponInventoryWidget::AddItemToWeaponInventory(const FName& Name, int32 WeaponInventoryIndex)
{
	auto WeaponIcon = FindTexture(Name);
	if (nullptr == WeaponIcon)
	{
		ZLOG(Error, TEXT("Invalid weapon.."));
		return;
	}

	switch (WeaponInventoryIndex)
	{
		case EWeaponSlot::Main1:
		{
			if (WeaponMainFirst)
			{
				WeaponMainFirst->SetBrushFromTexture(WeaponIcon);
				WeaponMainFirst->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		}
		case EWeaponSlot::Main2:
		{
			if (WeaponMainSecond)
			{
				WeaponMainSecond->SetBrushFromTexture(WeaponIcon);
				WeaponMainSecond->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		}
		case EWeaponSlot::Grenade:
		{
			if (WeaponGrenade)
			{
				WeaponGrenade->SetBrushFromTexture(WeaponIcon);
				WeaponGrenade->SetVisibility(ESlateVisibility::Visible);
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

void UZWeaponInventoryWidget::ClearWidget(int32 WeaponInventoryIndex)
{
	switch (WeaponInventoryIndex)
	{
	case EWeaponSlot::Main1:
	{
		if (WeaponMainFirst)
		{
			WeaponMainFirst->SetBrushFromTexture(nullptr);
			WeaponMainFirst->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	}
	case EWeaponSlot::Main2:
	{
		if (WeaponMainSecond)
		{
			WeaponMainSecond->SetBrushFromTexture(nullptr);
			WeaponMainSecond->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	}
	case EWeaponSlot::Grenade:
	{
		if (WeaponGrenade)
		{
			WeaponGrenade->SetBrushFromTexture(nullptr);
			WeaponGrenade->SetVisibility(ESlateVisibility::Hidden);
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

UTexture2D * const UZWeaponInventoryWidget::FindTexture(const FName & Name) const
{
	if (IconTable.Contains(Name))
	{
		return IconTable[Name];
	}

	return nullptr;
}

void UZWeaponInventoryWidget::LoadIconImage()
{
	if (nullptr == IconDataTable)
	{
		ZLOG(Error, TEXT("IconData not exist.."));
		return;
	}

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(MyGameInstance);

	TArray<FName> Names = IconDataTable->GetRowNames();

	for (const auto& Name : Names)
	{
		auto Data = IconDataTable->FindRow<FZImageData>(Name, TEXT(""));
		if (Data)
		{
			FSoftObjectPath Path(Data->ImagePath);
			MyGameInstance->AssetLoader.RequestSyncLoad(Path);

			TSoftObjectPtr<UTexture2D> Icon(Data->ImagePath);
			if (Icon.IsValid())
			{
				IconTable.Add(*Data->Name, Icon.Get());
			}

		}

	}
}
