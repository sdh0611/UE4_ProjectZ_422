// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBaseCharacter.h"
#include "ZZombie.generated.h"

DECLARE_DELEGATE(FOnAttackEnd);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZZombie : public AZBaseCharacter
{
	GENERATED_BODY()
	
public:
	AZZombie();

public:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

public:
	void Attack();
	void AttackEnd();

public:
	class UZCharacterAnimInstance* const GetZombieAnimInstance() const;

public:
	FOnAttackEnd OnAttackEnd;

private:
	UPROPERTY(VisibleAnywhere, Category = Zombie)
	float AttackDamage;

	UPROPERTY(VisibleAnywhere, Category = Zombie)
	bool bIsAttacking;

private:
	UPROPERTY()
	class USphereComponent* Sphere;

	UPROPERTY()
	class UPawnSensingComponent* Sense;

	//UPROPERTY()
	//class UZCharacterAnimInstance* ZombieAnimInstance;
};
