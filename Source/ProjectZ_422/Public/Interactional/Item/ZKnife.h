// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZWeapon.h"
#include "ZKnife.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZKnife : public AZWeapon
{
	GENERATED_BODY()
	
public:
	AZKnife();

public:
	virtual void Fire() override;

private:
	void AttackCheck();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRadius;

};
