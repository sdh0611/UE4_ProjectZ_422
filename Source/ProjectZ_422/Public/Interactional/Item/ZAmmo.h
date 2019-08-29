// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZItem.h"
#include "ZAmmo.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Default,
	Ammo556,
	Ammo768,
	AmmoGrenade
};

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZAmmo : public AZItem
{
	GENERATED_BODY()
	
public:
	AZAmmo();	
	
public:
	void SetAmmoType(EAmmoType NewAmmoType);

	EAmmoType GetAmmoType();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAmmoType AmmoType;

};
