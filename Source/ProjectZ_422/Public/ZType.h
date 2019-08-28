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
struct FZStaticMeshData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZStaticMeshData()
		:Name(TEXT("")), StaticMeshPath(TEXT(""))
	{

	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString StaticMeshPath;

};


USTRUCT(BlueprintType)
struct FZSkeletalMeshData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZSkeletalMeshData()
		:Name(TEXT("")), SkeletalMeshPath(TEXT(""))
	{

	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString SkeletalMeshPath;

};

/*
	ItemData
*/

USTRUCT(BlueprintType)
struct FZItemData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZItemData()
		: ItemName(TEXT("")), ItemWeight(0)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	int32 ItemWeight;	
};


USTRUCT(BlueprintType)
struct FZWeaponData : public FZItemData
{

	GENERATED_BODY()

public:
	FZWeaponData()
		: FZItemData(), Damage(0.f), FireDelay(1.f), MaxAmmo(30), ExplosionRadius(0.f), 
		WeaponCategory(TEXT("Invalid")), WeaponType(TEXT("Default"))
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	float FireDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString WeaponCategory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString WeaponType;

};

USTRUCT(BlueprintType)
struct FZRecoveryData : public FZItemData
{
	GENERATED_BODY()

public:
	FZRecoveryData()
		: FZItemData(), RecoveryAmount(0.f), RecoveryDelay(1.f)
	{
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	float RecoveryAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	float RecoveryDelay;

};

USTRUCT(BlueprintType)
struct FZDopingData : public FZItemData
{
	GENERATED_BODY()

public:
	FZDopingData()
		: FZItemData(), DopingAmount(0.f), DopingTime(1.f)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	float DopingAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	float DopingTime;

};

/*
	Shop data
*/

USTRUCT(BlueprintType)
struct FZShopItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FZShopItemData()
	{

	}

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	int32 ItemPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemExplanation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemImagePath;

};