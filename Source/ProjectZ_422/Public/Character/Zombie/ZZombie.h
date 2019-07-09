// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBaseCharacter.h"
#include "ZZombie.generated.h"

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
<<<<<<< HEAD

public:
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

public:
	void MileeAttack();

public:
	class UZCharacterAnimInstance* const GetZombieAnimInstance() const;

private:
	UPROPERTY(VisibleAnywhere, Category = Zombie)
	float AttackDamage;
=======

public:
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);
>>>>>>> origin/branch0621


private:
	UPROPERTY()
	class USphereComponent* Sphere;

	UPROPERTY()
	class UPawnSensingComponent* Sense;

	//UPROPERTY()
	//class UZCharacterAnimInstance* ZombieAnimInstance;
};
