// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZProjectile.h"
#include "ZBulletProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZBulletProjectile : public AZProjectile
{
	GENERATED_BODY()
	
public:
	AZBulletProjectile();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void FireInDirection(const FVector& Direction) override;

public:
	void SetBulletSpeed(float NewSpeed);
	void SetBulletLifeSpan(float NewLifeSpan);

private:
	void TraceBullet();

protected:
	UPROPERTY(VisibleAnywhere)
	FVector PreLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMaterialInterface* Decal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* ProjectileTrailParticle;

};
