// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ZType.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZType : public UObject
{
	GENERATED_BODY()
	
};

/*
	Physics material 관련 매크로
*/
#define SURFACE_HEAD SurfaceType1 
#define SURFACE_BODY SurfaceType2


/*
	Mesh관련 데이터형
*/

USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZStaticMeshData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZStaticMeshData()
		:Name(TEXT(""))
	{

	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	TSoftObjectPtr<class UStaticMesh> StaticMeshPath;

};


USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZSkeletalMeshData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZSkeletalMeshData()
		:Name(TEXT(""))
	{

	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	TSoftObjectPtr<class USkeletalMesh> SkeletalMeshPath;

};


USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZImageData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZImageData()
		:Name(TEXT(""))
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	TSoftObjectPtr<class UTexture2D> ImagePath;

};

USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZSoundData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZSoundData()
		:Name(TEXT(""))
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	TSoftObjectPtr<class USoundBase> SoundPath;

};

/*
	ItemData
*/

USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FZItemData()
		: ItemName(TEXT("")), ItemWeight(0), MaxQuantity(30)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	int32 ItemWeight;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	int32 MaxQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FText ItemExplanation;

};


USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZWeaponData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZWeaponData()
		: Damage(0.f), WeaponCategory(TEXT("Invalid"))
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString WeaponCategory;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	//TSoftObjectPtr<class USoundBase> FireSound;
};

/* 
	NOTE(8.29) :
		WeaponData를 WeaponCategory에 맞게 세분화시킴.
*/
USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZGunData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZGunData()
		: FireDelay(1.f), MaxAmmo(30), UseAmmoName(TEXT("")), 
		GunType(TEXT("Default")), BulletSpeed(10000.f)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float FireDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString UseAmmoName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString GunType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float BulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float BulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float SpreadDecrement;

};


USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZGrenadeData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZGrenadeData()
		: ExplosionDelay(1.f), ExplosionRadius(0.f)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float ExplosionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float ExplosionRadius;

};



USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZRecoveryData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FZRecoveryData()
		: RecoveryAmount(0.f), RecoveryDelay(1.f)
	{
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float RecoveryAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float RecoveryDelay;

};

USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZDopingData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FZDopingData()
		: DopingAmount(0.f), DopingTime(1.f)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float DopingAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	float DopingTime;

};

/*
	Shop data
*/

USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZShopItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FZShopItemData()
		:ShopID(-1)
	{

	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	int32 ShopID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	int32 ItemPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	FString ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
	bool bIsDealOnlyOne;

};


