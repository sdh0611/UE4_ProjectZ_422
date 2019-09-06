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
};


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

public:
	int32 GetWeaponInventoryIndex() const;
	bool IsEquipped() const;
	EWeaponCategory GetWeaponCategory() const;
	virtual class UAnimMontage* const GetFireAnimMontage() const;

protected:
	FHitResult WeaponTrace(float Distance, bool bDrawDebugLine = false);

public:
	FOnWeaponFired OnWeaponFired;

	/*
		현재 Player가 들고 있는 무기인지의 여부
	*/
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bIsEquipped;

protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 WeaponInventoryIndex;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float TraceDistance;

	UPROPERTY(EditAnywhere, Category = Weapon, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	EWeaponCategory WeaponCategory;

	UPROPERTY(EditAnywhere, Category = Weapon)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
	bool bToggleDebug;

};
