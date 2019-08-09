// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZProjectile.h"
#include "ZGrenadeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZGrenadeProjectile : public AZProjectile
{
	GENERATED_BODY()
	
public:
	AZGrenadeProjectile();

public:
	virtual void FireInDirection(const FVector& Direction);

public:
	void SetFireDelay(float NewFireDelay);
	void SetDamageCauser(class AController* DamageInstigator);

private:
	void Explosion();

private:
	FTimerHandle ExplosionTimer;

private:
	UPROPERTY(EditAnywhere, Category = Grenade)
	float FireDelay;

private:
	UPROPERTY(VisibleAnywhere, Category = Grenade)
	class UParticleSystemComponent* Particle;

	UPROPERTY()
	class AController* DamageCauser;

	//UPROPERTY(VisibleAnywhere, Category = Grenade)
	//class UParticleSystem* PSGrenade;

};
