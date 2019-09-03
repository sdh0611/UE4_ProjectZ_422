// Fill out your copyright notice in the Description page of Project Settings.


#include "ZKnife.h"
#include "Engine/World.h"


AZKnife::AZKnife()
{
	WeaponCategory = EWeaponCategory::Knife;

}

void AZKnife::Fire()
{
	TArray<FHitResult> Hits;




	Super::Fire();
}
