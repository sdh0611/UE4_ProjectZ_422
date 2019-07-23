// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Character/ZCharacterAnimInstance.h"
#include "ZZombieAnimInstance.generated.h"

DECLARE_DELEGATE(FOnAttackCheck);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZZombieAnimInstance : public UZCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	UZZombieAnimInstance();

public:
	UFUNCTION()
	void AnimNotify_ZombieAttackCheck();

	UFUNCTION()
	void AnimNotify_ZombieAttackEndCheck();

public:
	/* Setter */
	void SetIsAttacking(bool NewState);

	/* Getter */
	bool IsAttacking() const;
	
public:
	FOnAttackCheck OnAttackCheck;

private:
	bool bIsAttacking;

};
