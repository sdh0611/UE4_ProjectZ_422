// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponFired);

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

public:
	void InitWeaponData(const FZWeaponData* const NewWeaponData);
	void Reload();
	bool IsCanReload() const;

public:
	void SetWeaponInventoryIndex(int32 NewIndex);
	void SetIsEquipped(bool NewState);
	void SetWantsToFire(bool NewState);
	void SetIsReloading(bool NewState);
	void SetCurrentAmmo(int32 NewAmmo);
	void SetMaxAmmo(int32 NewAmmo);


public:
	int32 GetWeaponInventoryIndex() const;
	bool IsEquipped() const;
	bool IsWantsToFire() const;
	bool IsReloading() const;
	int32 GetCurrentAmmo() const;
	int32 GetMaxAmmo() const;


protected:
	virtual void Fire();
	bool CheckNeedToReload();

public:
	FOnWeaponFired OnWeaponFired;

private:
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

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bWantsToFire;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bIsReloading;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireTimer;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireDelay;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	EWeaponType WeaponType;


};
