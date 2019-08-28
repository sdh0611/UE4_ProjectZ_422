// Fill out your copyright notice in the Description page of Project Settings.


#include "ZRecovery.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"

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

void AZRecovery::OnUsed()
{
	ItemOwner->GetStatusComponent()->AdjustCurrentHP(RecoveryAmount);

	AdjustQuantity(-1);
}
