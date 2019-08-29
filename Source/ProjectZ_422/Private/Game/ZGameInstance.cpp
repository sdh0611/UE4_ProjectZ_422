// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameInstance.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"

UZGameInstance::UZGameInstance()
{

}

void UZGameInstance::Init()
{
	Super::Init();

	LoadStaticMesh();
	LoadSkeletalMesh();
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

const FZStaticMeshData * const UZGameInstance::GetStaticMeshData(const FString & MeshName)
{
	auto Names = StaticMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = StaticMeshDataTable->FindRow<FZStaticMeshData>(Name, TEXT(""));
		if (Data->Name == MeshName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZSkeletalMeshData * const UZGameInstance::GetSkeletalMeshData(const FString & MeshName)
{
	auto Names = SkeletalMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = SkeletalMeshDataTable->FindRow<FZSkeletalMeshData>(Name, TEXT(""));
		if (Data->Name == MeshName)
		{
			return Data;
		}
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

const FZGunData * const UZGameInstance::GetGunDataByRowIndex(int32 RowIndex)
{
	return GunDataTable->FindRow<FZGunData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZGunData * const UZGameInstance::GetGunDataByName(const FString & GunName)
{
	auto Names = GunDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = GunDataTable->FindRow<FZGunData>(Name, TEXT(""));
		if (Data->ItemName == GunName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZGrenadeData * const UZGameInstance::GetGrenadeDataByRowIndex(int32 RowIndex)
{
	return GrenadeDataTable->FindRow<FZGrenadeData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZGrenadeData * const UZGameInstance::GetGrenadeDataByName(const FString & GrenadeName)
{
	auto Names = GrenadeDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = GrenadeDataTable->FindRow<FZGrenadeData>(Name, TEXT(""));
		if (Data->ItemName == GrenadeName)
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

const FZItemData * const UZGameInstance::GetAmmoDataByRowIndex(int32 RowIndex)
{
	return AmmoDataTable->FindRow<FZItemData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZItemData * const UZGameInstance::GetAmmoDataByName(const FString & AmmoName)
{
	auto Names = AmmoDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = AmmoDataTable->FindRow<FZItemData>(Name, TEXT(""));
		if (Data->ItemName == AmmoName)
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

void UZGameInstance::LoadStaticMesh()
{
	if (nullptr == StaticMeshDataTable)
	{
		ZLOG(Error, TEXT("StaticMeshDataTable not exist.."));
		return;
	}

	TArray<FName> Names = StaticMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = StaticMeshDataTable->FindRow<FZStaticMeshData>(Name, TEXT(""));

		FSoftObjectPath ObjectPath(Data->StaticMeshPath);
		AssetLoader.RequestSyncLoad(ObjectPath);

		TSoftObjectPtr<UStaticMesh> StaticMesh(Data->StaticMeshPath);
		if (StaticMesh.IsValid())
		{
			StaticMeshTable.Add(Data->Name, StaticMesh.Get());
		}
	}

}

void UZGameInstance::LoadSkeletalMesh()
{
	if (nullptr == SkeletalMeshDataTable)
	{
		ZLOG(Error, TEXT("SkeletalMeshDataTable not exist.."));
		return;
	}
	
	TArray<FName> Names = SkeletalMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = SkeletalMeshDataTable->FindRow<FZSkeletalMeshData>(Name, TEXT(""));

		FSoftObjectPath ObjectPath(Data->SkeletalMeshPath);
		AssetLoader.RequestSyncLoad(ObjectPath);

		TSoftObjectPtr<USkeletalMesh> SkeletalMesh(Data->SkeletalMeshPath);
		if (SkeletalMesh.IsValid())
		{
			SkeletalMeshTable.Add(Data->Name, SkeletalMesh.Get());
		}
	}

}
