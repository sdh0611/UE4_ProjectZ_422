// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZWeapon.h"
#include "ZGun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZGun : public AZWeapon
{
	GENERATED_BODY()
	
public:
	AZGun();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Fire();


};
