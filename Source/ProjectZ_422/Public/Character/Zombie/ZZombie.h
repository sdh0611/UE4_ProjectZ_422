// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBaseCharacter.h"
#include "ZBaseZombie.h"
#include "ZZombie.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZZombie : public AZBaseZombie
{
	GENERATED_BODY()
	
public:
	AZZombie();

public:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	virtual void Revive() override;

public:
	virtual void SetActive(bool bActive) override;

public:
	virtual void ChangeZombieState(EZombieState NewState) override;

private:
	virtual void AttackCheck() override;
	virtual void OnDead() override;
	virtual void OnSensingPlayer(APawn* Pawn) override;

};
