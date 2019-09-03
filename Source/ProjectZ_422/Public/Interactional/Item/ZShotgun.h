// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZGun.h"
#include "ZShotgun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZShotgun : public AZGun
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;


};
