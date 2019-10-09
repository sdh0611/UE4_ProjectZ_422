// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZWeapon.h"
#include "ZUsableItemInterface.h"
#include "ZGrenade.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZGrenade : public AZWeapon, public IZUsableItemInterface
{
	GENERATED_BODY()
	
public:
	AZGrenade();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void InitItemData(const FZItemData* const NewItemData) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnUsed() override;

public:
	/*
		AnimMontage�� �ִ� ��Ƽ���� Ÿ�ֿ̹� ����� �޼ҵ�.
	*/
	void ThrowGrenade();

public:
	void SetIsThrown(bool NewState);

public:
	bool IsThrown() const;

protected:
	virtual void Fire() override;

protected:
	// ��ݽ� �����Ǵ� ����ź�� Ŭ����.
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AZGrenadeProjectile> GrenadeProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = Grenade)
	bool bIsThrown;

	UPROPERTY(EditAnywhere, Category = Grenade)
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, Category = Grenade)
	float ExplosionDelay;

};
