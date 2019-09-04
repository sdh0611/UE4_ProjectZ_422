// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZGun.h"
#include "ZAR.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZAR : public AZGun
{
	GENERATED_BODY()

public:
	AZAR();

public:
	virtual void Fire() override;


protected:

	
};
