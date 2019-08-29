// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZWeapon.h"
#include "ZGun.generated.h"

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

public:
	bool IsCanReload() const;
	bool IsReloading() const;
	bool IsWantsToFire() const;
	int32 GetCurrentAmmo() const;
	int32 GetMaxAmmo() const;

protected:
	bool CheckNeedToReload();

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
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bWantsToFire;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireTimer;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireDelay;
};
