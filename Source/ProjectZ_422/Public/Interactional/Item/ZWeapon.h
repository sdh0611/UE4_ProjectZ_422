// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponFired);

UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
	Gun,
	Knife,
	Grenade,
	Invalid
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AR,
	SR,
	DMR,
	SMG,
	Shotgun,
	Pistol,
	Knife,
	Grenade,
	Invalid
};


/*
	WeaponCategory
*/

static FString GetWeaponCategoryEnumAsString(EWeaponCategory WeaponCategory)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponCategory"), true);
	if (nullptr == EnumPtr)
	{
		return FString(TEXT("Invalid"));
	}

	FString Concated = EnumPtr->GetNameStringByValue((int64)WeaponCategory);
	Concated.RemoveFromStart(TEXT("EWeaponCategory::"));

	return Concated;
}

static EWeaponCategory GetWeaponCategoryFromString(const FString& WeaponCategoryName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponCategory"), true);
	if (nullptr == EnumPtr)
	{
		return EWeaponCategory::Invalid;
	}

	if (WeaponCategoryName == TEXT("Gun"))
	{
		return EWeaponCategory::Gun;
	}
	else if (WeaponCategoryName == TEXT("Knife"))
	{
		return EWeaponCategory::Knife;
	}
	else if (WeaponCategoryName == TEXT("Grenade"))
	{
		return EWeaponCategory::Grenade;
	}

	return EWeaponCategory::Invalid;
}


/*
	WeaponType
*/

static FString GetWeaponTypeEnumAsString(EWeaponType WeaponType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
	if (nullptr == EnumPtr)
	{
		return FString(TEXT("Invalid"));
	}

	FString Concated = EnumPtr->GetNameStringByValue((int64)WeaponType);
	Concated.RemoveFromStart(TEXT("EWeaponType::"));

	return Concated;
}

static EWeaponType GetWeaponTypeFromString(const FString& WeaponTypeName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
	if (nullptr == EnumPtr)
	{
		return EWeaponType::Invalid;
	}

	if (WeaponTypeName == TEXT("SMG"))
	{
		return EWeaponType::SMG;
	}
	else if (WeaponTypeName == TEXT("AR"))
	{
		return EWeaponType::AR;
	}
	else if (WeaponTypeName == TEXT("Shotgun"))
	{
		return EWeaponType::Shotgun;
	}
	else if (WeaponTypeName == TEXT("DMR"))
	{
		return EWeaponType::DMR;
	}
	else if (WeaponTypeName == TEXT("SR"))
	{
		return EWeaponType::SR;
	}
	else if (WeaponTypeName == TEXT("Pistol"))
	{
		return EWeaponType::Pistol;
	}
	else if (WeaponTypeName == TEXT("Grenade"))
	{
		return EWeaponType::Grenade;
	}
	else if (WeaponTypeName == TEXT("Knife"))
	{
		return EWeaponType::Knife;
	}

	return EWeaponType::Invalid;
}


/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZWeapon : public AZItem
{
	GENERATED_BODY()
	
public:
	AZWeapon();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRemoved() override;
	virtual void OnDropped() override;
	virtual void InitItemData(const struct FZItemData* const NewItemData);
	virtual void Fire();
	virtual void FireEnd();

public:
	void SetWeaponInventoryIndex(int32 NewIndex);
	void SetIsEquipped(bool NewState);
	void SetWeaponCategory(EWeaponCategory NewWeaponCategory);
	void SetWeaponType(EWeaponType NewWeaponType);

public:
	int32 GetWeaponInventoryIndex() const;
	bool IsEquipped() const;
	EWeaponCategory GetWeaponCategory() const;
	EWeaponType GetWeaponType() const;

protected:
	FHitResult WeaponTrace(float Distance, bool bDrawDebugLine = false);

public:
	FOnWeaponFired OnWeaponFired;

protected:
	// 사격시 생성되는 총알의 클래스.
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AZProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 WeaponInventoryIndex;

	/*
		현재 Player가 들고 있는 무기인지의 여부
	*/
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bIsEquipped;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireDelay;

	UPROPERTY(EditAnywhere, Category = Weapon, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	EWeaponCategory WeaponCategory;

	UPROPERTY(EditAnywhere, Category = Weapon, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	EWeaponType WeaponType;


};
