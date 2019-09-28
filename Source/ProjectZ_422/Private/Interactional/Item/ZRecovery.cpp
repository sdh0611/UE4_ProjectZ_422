// Fill out your copyright notice in the Description page of Project Settings.


#include "ZRecovery.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "ZGameInstance.h"
#include "UnrealNetwork.h"


AZRecovery::AZRecovery()
{
	ItemType = EItemType::Recovery;

	// Code to test
	ItemName = TEXT("Recovery");

	RecoveryAmount = 10.f;
	RecoveryDelay = 1.f;
}

void AZRecovery::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZRecovery, RecoveryAmount);
	DOREPLIFETIME(AZRecovery, RecoveryDelay);

}

void AZRecovery::InitItemData(const FZItemData * NewItemData)
{
	Super::InitItemData(NewItemData);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(MyGameInstance);

	auto NewRecoveryData = MyGameInstance->GetRecoveryDataByName(NewItemData->ItemName);
	if (nullptr == NewRecoveryData)
	{
		ZLOG(Error, TEXT("Invalid recovery data."));
		return;
	}

	RecoveryAmount = NewRecoveryData->RecoveryAmount;
	RecoveryDelay = NewRecoveryData->RecoveryDelay;

}

void AZRecovery::OnUsed()
{
	if (!HasAuthority())
	{
		ServerUseItem();
	}
}

bool AZRecovery::ServerUseItem_Validate()
{
	return true;
}

void AZRecovery::ServerUseItem_Implementation()
{
	//ItemOwner->GetStatusComponent()->AdjustCurrentHP(RecoveryAmount);

	AdjustQuantity(-1);
}
