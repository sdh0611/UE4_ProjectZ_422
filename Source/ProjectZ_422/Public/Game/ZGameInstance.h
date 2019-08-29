// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "ZGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UZGameInstance();

public:
	virtual void Init() override;

public:
	/*
		MeshTable 관련
	*/
	class UStaticMesh* const GetStaticMesh(const FString& Name);
	class USkeletalMesh* const GetSkeletalMesh(const FString& Name);

	const FZStaticMeshData* const GetStaticMeshData(const FString& MeshName);
	const FZSkeletalMeshData* const GetSkeletalMeshData(const FString& MeshName);

	/*
		WeaponTable 관련
	*/
	const FZWeaponData* const GetWeaponDataByRowIndex(int32 RowIndex);
	const FZWeaponData* const GetWeaponDataByName(const FString& WeaponName);

	const FZGunData* const GetGunDataByRowIndex(int32 RowIndex);
	const FZGunData* const GetGunDataByName(const FString& GunName);

	const FZGrenadeData* const GetGrenadeDataByRowIndex(int32 RowIndex);
	const FZGrenadeData* const GetGrenadeDataByName(const FString& GrenadeName);

	/*
		RecoveryTable 관련
	*/
	const FZRecoveryData* const GetRecoveryDataByRowIndex(int32 RowIndex);
	const FZRecoveryData* const GetRecoveryDataByName(const FString& RecoveryName);

	/*
		DopingTable 관련
	*/
	const FZDopingData* const GetDopingDataByRowIndex(int32 RowIndex);
	const FZDopingData* const GetDopingDataByName(const FString& DopingName);

	/*
		AmmoTable 관련
	*/
	const FZItemData* const GetAmmoDataByRowIndex(int32 RowIndex);
	const FZItemData* const GetAmmoDataByName(const FString& AmmoName);

	/*
		ShopTable 관련
	*/
	class UDataTable* const GetShopItemDataTable() const;
	const FZShopItemData* const GetShopItemDataByName(const FString& ShopItemName);

private:
	/* 애셋 로딩 요청 */
	void LoadStaticMesh();
	void LoadSkeletalMesh();

public:
	FStreamableManager AssetLoader;

protected:
	UPROPERTY(EditAnywhere, Category = MeshDataTable)
	class UDataTable* StaticMeshDataTable;

	UPROPERTY(EditAnywhere, Category = MeshDataTable)
	class UDataTable* SkeletalMeshDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* GunDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* GrenadeDataTable;

	UPROPERTY(EditAnywhere, Category = RecoveryDataTable)
	class UDataTable* RecoveryDataTable;

	UPROPERTY(EditAnywhere, Category = DopingDataTable)
	class UDataTable* DopingDataTable;

	UPROPERTY(EditAnywhere, Category = AmmoDataTable)
	class UDataTable* AmmoDataTable;

	UPROPERTY(EditAnywhere, Category = ShopDataTable)
	class UDataTable* ShopItemDataTable;

private:
	UPROPERTY(VisibleAnywhere, Category = MeshTable)
	TMap<FString, class UStaticMesh*> StaticMeshTable;

	UPROPERTY(VisibleAnywhere, Category = MeshTable)
	TMap<FString, class USkeletalMesh*> SkeletalMeshTable;



};
