// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZWeapon.h"
#include "ZGun.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	AutoFire = 0,
	//InterruptedFire,
	SingleShot
};

UENUM(BlueprintType)
enum class EGunType : uint8
{
	Default = 0,
	AR,
	SR,
	DMR,
	SMG,
	Shotgun,
	Invalid
};

/*
	GunType
*/

static FString GetGunTypeEnumAsString(EGunType GunType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGunType"), true);
	if (nullptr == EnumPtr)
	{
		return FString(TEXT("Invalid"));
	}

	FString Concated = EnumPtr->GetNameStringByValue((int64)GunType);
	Concated.RemoveFromStart(TEXT("EGunType::"));

	return Concated;
}

static EGunType GetGunTypeFromString(const FString& GunTypeName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGunType"), true);
	if (nullptr == EnumPtr)
	{
		return EGunType::Invalid;
	}

	if (GunTypeName == TEXT("SMG"))
	{
		return EGunType::SMG;
	}
	else if (GunTypeName == TEXT("AR"))
	{
		return EGunType::AR;
	}
	else if (GunTypeName == TEXT("Shotgun"))
	{
		return EGunType::Shotgun;
	}
	else if (GunTypeName == TEXT("DMR"))
	{
		return EGunType::DMR;
	}
	else if (GunTypeName == TEXT("SR"))
	{
		return EGunType::SR;
	}

	return EGunType::Invalid;
}

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZGun : public AZWeapon
{
	GENERATED_BODY()
	
public:
	AZGun();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void InitItemData(const FZItemData* const NewItemData) override;
	virtual void Fire() override;
	virtual void FireEnd() override;

public:
	void Reload();

public:
	void SetIsReloading(bool NewState);
	void SetCurrentAmmo(int32 NewAmmo);
	void SetMaxAmmo(int32 NewAmmo);
	void SetWantsToFire(bool NewState);
	void SetFireMode(EFireMode NewMode);

public:
	bool IsCanReload() const;
	bool IsReloading() const;
	bool IsWantsToFire() const;
	int32 GetCurrentAmmo() const;
	int32 GetMaxAmmo() const;
	const FString& GetUseAmmoName() const;
	EGunType GetGunType() const;
	EFireMode GetFireMode() const;
	virtual class UAnimMontage* const GetFireAnimMontage() const override;

protected:
	bool CheckNeedToReload();

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bWantsToFire;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	class UParticleSystem* FireEffect;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	class USoundBase* EmptySound;

	
	UPROPERTY(EditAnywhere, Category = Weapon)
	FName EffectAttachSocketName;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AZBulletProjectile> BulletClass;


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bIsFiring;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireTimer;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireDelay;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float BulletSpeed;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float BulletLifeSpan;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float BulletSpread;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float SpreadDecrement;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float CurrentSpread;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	FString UseAmmoName;

	UPROPERTY(EditAnywhere, Category = Weapon)
	EGunType GunType;

	UPROPERTY(EditAnywhere, Category = Weapon)
	EFireMode FireMode;

};
