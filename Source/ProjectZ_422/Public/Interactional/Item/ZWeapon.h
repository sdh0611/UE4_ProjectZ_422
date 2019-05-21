// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZWeapon.generated.h"

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
	void SetWeaponInventoryIndex(int32 NewIndex);
	void SetIsEquipped(bool NewState);
	void SetWantsToFire(bool NewState);

public:
	int32 GetWeaponInventoryIndex() const;
	bool IsEquipped() const;
	bool IsWantsToFire() const;

private:
	void Fire();

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

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireDelay;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireTimer;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool bWantsToFire;

};
