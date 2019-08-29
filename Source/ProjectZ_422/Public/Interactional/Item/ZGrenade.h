// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZWeapon.h"
#include "ZGrenade.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZGrenade : public AZWeapon
{
	GENERATED_BODY()
	
public:
	AZGrenade();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void InitItemData(const FZItemData* const NewItemData) override;
	virtual void Fire() override;

public:
	/*
		AnimMontage의 애님 노티파이 타이밍에 실행될 메소드.
	*/
	void ThrowGrenade();

public:
	void SetIsThrown(bool NewState);

public:
	bool IsThrown() const;

protected:
	// 사격시 생성되는 수류탄의 클래스.
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AZGrenadeProjectile> GrenadeProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = Grenade)
	bool bIsThrown;

	UPROPERTY(EditAnywhere, Category = Grenade)
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, Category = Grenade)
	float ExplosionDelay;

};
