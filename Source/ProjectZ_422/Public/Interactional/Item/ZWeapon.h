// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponFired);

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

public:
	void Reload();
	bool IsCanReload() const;

public:
	void SetWeaponInventoryIndex(int32 NewIndex);
	void SetIsEquipped(bool NewState);
	void SetWantsToFire(bool NewState);
	void SetIsReloading(bool NewState);

public:
	int32 GetWeaponInventoryIndex() const;
	bool IsEquipped() const;
	bool IsWantsToFire() const;
	bool IsReloading() const;

private:
	void Fire();
	bool CheckNeedToReload();

public:
	FOnWeaponFired OnWeaponFired;

private:
	// ��ݽ� �����Ǵ� �Ѿ��� Ŭ����.
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AZProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 WeaponInventoryIndex;

	/*
		���� Player�� ��� �ִ� ���������� ����
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
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 MaxAmmo;



};
