// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "ZItem.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

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
	void SetUserID(const FString& NewID);
	void SetUserNickname(const FString& NewNickname);

	const FString& GetUserNickname() const;

public:
	/* Login server ��� ����. */
	void HttpPostLogin(FString URL, const FString& NewUserID, const FString& NewUserPW, FHttpRequestCompleteDelegate RequestDelegate);
	void HttpPostLogout(FString URL, FHttpRequestCompleteDelegate RequestDelegate);

public:
	/*
		MeshTable ����
	*/
	class UStaticMesh* const GetStaticMesh(const FString& Name);
	class USkeletalMesh* const GetSkeletalMesh(const FString& Name);
	class UTexture2D* const GetItemImage(const FString& Name);

	/*
		��ü ItemData �˻�
	*/
	const FZItemData* const GetItemDataByName(const FString& ItemName);

	/*
		WeaponTable ����
	*/
	const FZWeaponData* const GetWeaponDataByRowIndex(int32 RowIndex);
	const FZWeaponData* const GetWeaponDataByName(const FString& WeaponName);

	const FZGunData* const GetGunDataByRowIndex(int32 RowIndex);
	const FZGunData* const GetGunDataByName(const FString& GunName);

	const FZGrenadeData* const GetGrenadeDataByRowIndex(int32 RowIndex);
	const FZGrenadeData* const GetGrenadeDataByName(const FString& GrenadeName);

	/*
		RecoveryTable ����
	*/
	const FZRecoveryData* const GetRecoveryDataByRowIndex(int32 RowIndex);
	const FZRecoveryData* const GetRecoveryDataByName(const FString& RecoveryName);

	/*
		DopingTable ����
	*/
	const FZDopingData* const GetDopingDataByRowIndex(int32 RowIndex);
	const FZDopingData* const GetDopingDataByName(const FString& DopingName);

	/*
		AmmoTable ����
	*/
	const FZItemData* const GetAmmoDataByRowIndex(int32 RowIndex);
	const FZItemData* const GetAmmoDataByName(const FString& AmmoName);

	/*
		ShopTable ����
	*/
	class UDataTable* const GetShopItemDataTable() const;
	const FZShopItemData* const GetShopItemDataByName(const FString& ShopItemName);

private:
	/* �ּ� �ε� */
	void LoadStaticMesh();
	void LoadSkeletalMesh();
	void LoadImage();

	/* �� �ε� */
	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMap(UWorld* NewWorld);


public:
	FStreamableManager AssetLoader;

	bool bIsVerified = false;

private:
	FString UserID;
	
	FString Nickname;


protected:
	class FHttpModule* Http;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString HostName = TEXT("127.0.0.1:8000");

protected:
	UPROPERTY(EditAnywhere, Category = MeshDataTable)
	class UDataTable* StaticMeshDataTable;

	UPROPERTY(EditAnywhere, Category = MeshDataTable)
	class UDataTable* SkeletalMeshDataTable;

	UPROPERTY(EditAnywhere, Category = ImageDataTable)
	class UDataTable* ItemImageDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* ItemDataTable;

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

	UPROPERTY(VisibleAnywhere, Category = ImageTable)
	TMap<FString, class UTexture2D*> ItemImageTable;


};
