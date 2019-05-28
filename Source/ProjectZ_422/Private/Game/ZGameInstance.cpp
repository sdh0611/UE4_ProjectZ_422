// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameInstance.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"


UZGameInstance::UZGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_SM(TEXT("DataTable'/Game/GameData/StaticMeshData.StaticMeshData'"));
	if (DT_SM.Succeeded())
	{
		StaticMeshDataTable = DT_SM.Object;
		InitStaticMeshTable();
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_SK(TEXT("DataTable'/Game/GameData/SkeletalMeshData.SkeletalMeshData'"));
	if (DT_SK.Succeeded())
	{
		SkeletalMeshDataTable = DT_SK.Object;
		InitSkeletalMeshTable();
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_WEAPON(TEXT("DataTable'/Game/GameData/WeaponData.WeaponData'"));
	if(DT_WEAPON.Succeeded())
	{
		WeaponDataTable = DT_WEAPON.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_RECOVERY(TEXT("DataTable'/Game/GameData/RecoveryData.RecoveryData'"));
	if (DT_RECOVERY.Succeeded())
	{
		RecoveryDataTable = DT_RECOVERY.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_DOPING(TEXT("DataTable'/Game/GameData/DopingData.DopingData'"));
	if (DT_DOPING.Succeeded())
	{
		DopingDataTable = DT_DOPING.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_SHOP(TEXT("DataTable'/Game/GameData/ShopItemData.ShopItemData'"));
	if (DT_SHOP.Succeeded())
	{
		ShopItemDataTable = DT_SHOP.Object;
	}

}

void UZGameInstance::Init()
{
	Super::Init();

}

UStaticMesh * const UZGameInstance::GetStaticMesh(const FString & Name)
{
	if (StaticMeshTable.Contains(Name))
	{
		return StaticMeshTable[Name];
	}

	return nullptr;
}

USkeletalMesh * const UZGameInstance::GetSkeletalMesh(const FString & Name)
{
	if (SkeletalMeshTable.Contains(Name))
	{
		return SkeletalMeshTable[Name];
	}

	return nullptr;
}

const FZWeaponData * const UZGameInstance::GetWeaponDataByRowIndex(int32 RowIndex)
{
	return WeaponDataTable->FindRow<FZWeaponData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZWeaponData * const UZGameInstance::GetWeaponDataByName(const FString & WeaponName)
{
	auto Names = WeaponDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = WeaponDataTable->FindRow<FZWeaponData>(Name, TEXT(""));
		if (Data->ItemName == WeaponName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZRecoveryData * const UZGameInstance::GetRecoveryDataByRowIndex(int32 RowIndex)
{
	return RecoveryDataTable->FindRow<FZRecoveryData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZRecoveryData * const UZGameInstance::GetRecoveryDataByName(const FString & RecoveryName)
{
	auto Names = RecoveryDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = RecoveryDataTable->FindRow<FZRecoveryData>(Name, TEXT(""));
		if (Data->ItemName == RecoveryName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZDopingData * const UZGameInstance::GetDopingDataByRowIndex(int32 RowIndex)
{
	return DopingDataTable->FindRow<FZDopingData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZDopingData * const UZGameInstance::GetDopingDataByName(const FString & DopingName)
{
	auto Names = DopingDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = DopingDataTable->FindRow<FZDopingData>(Name, TEXT(""));
		if (Data->ItemName == DopingName)
		{
			return Data;
		}
	}

	return nullptr;
}

UDataTable * const UZGameInstance::GetShopItemDataTable() const
{
	return ShopItemDataTable;
}

const FZShopItemData * const UZGameInstance::GetShopItemDataByName(const FString & ShopItemName)
{
	auto Names = ShopItemDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = ShopItemDataTable->FindRow<FZShopItemData>(Name, TEXT(""));
		if (Data->ItemName == ShopItemName)
		{
			return Data;
		}
	}

	return nullptr;
}

void UZGameInstance::InitStaticMeshTable()
{
	// Init static mesh table
	TArray<FName> Names = StaticMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = StaticMeshDataTable->FindRow<FZStaticMeshData>(Name, TEXT(""));
		ConstructorHelpers::FObjectFinder<UStaticMesh> SM(*Data->StaticMeshPath);
		if (SM.Succeeded())
		{
			StaticMeshTable.Add(Data->Name, SM.Object);
		}
	}

}

void UZGameInstance::InitSkeletalMeshTable()
{
	// Init skeletal mesh table
	TArray<FName> Names = SkeletalMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = SkeletalMeshDataTable->FindRow<FZSkeletalMeshData>(Name, TEXT(""));
		ConstructorHelpers::FObjectFinder<USkeletalMesh> SK(*Data->SkeletalMeshPath);
		if (SK.Succeeded())
		{
			SkeletalMeshTable.Add(Data->Name, SK.Object);
		}
	}

}
