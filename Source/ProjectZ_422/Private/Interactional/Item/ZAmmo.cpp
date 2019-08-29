// Fill out your copyright notice in the Description page of Project Settings.


#include "ZAmmo.h"

AZAmmo::AZAmmo()
{
	ItemType = EItemType::Ammo;
	AmmoType = EAmmoType::Default;
}

void AZAmmo::SetAmmoType(EAmmoType NewAmmoType)
{
	AmmoType = NewAmmoType;
}

EAmmoType AZAmmo::GetAmmoType()
{
	return AmmoType;
}
