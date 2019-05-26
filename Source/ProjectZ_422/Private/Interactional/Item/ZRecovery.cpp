// Fill out your copyright notice in the Description page of Project Settings.


#include "ZRecovery.h"

AZRecovery::AZRecovery()
{

	ItemType = EItemType::Recovery;

	// Code to test
	ItemName = TEXT("Recovery");

	RecoveryAmount = 10.f;
	RecoveryDelay = 1.f;
}

void AZRecovery::InitItemData(const FZItemData * NewItemData)
{
	Super::InitItemData(NewItemData);

	auto NewRecoveryData = static_cast<const FZRecoveryData*>(NewItemData);

	RecoveryAmount = NewRecoveryData->RecoveryAmount;
	RecoveryDelay = NewRecoveryData->RecoveryDelay;

}
