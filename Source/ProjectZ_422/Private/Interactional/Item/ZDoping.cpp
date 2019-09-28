// Fill out your copyright notice in the Description page of Project Settings.


#include "ZDoping.h"
#include "ZCharacter.h"
#include "ZPlayerStatusComponent.h"
#include "ZGameInstance.h"

AZDoping::AZDoping()
{
	ItemType = EItemType::Doping;

	// Code to test
	ItemName = TEXT("Doping");
}

void AZDoping::InitItemData(const FZItemData * NewItemData)
{
	Super::InitItemData(NewItemData);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(nullptr != MyGameInstance);

	auto NewDopingData = MyGameInstance->GetDopingDataByName(NewItemData->ItemName);
	if (nullptr == NewDopingData)
	{
		ZLOG(Error, TEXT("Invalid doping data."));
		return;
	}

	DopingAmount = NewDopingData->DopingAmount;
	DopingTime = NewDopingData->DopingTime;

}

void AZDoping::OnUsed()
{
	ServerUseItem();
}

bool AZDoping::ServerUseItem_Validate()
{
	return true;
}

void AZDoping::ServerUseItem_Implementation()
{
	if (ItemOwner->GetPlayerStatusComponent()->GetDopingGageRatio() >= 1.f)
	{
		return;
	}

	ItemOwner->GetPlayerStatusComponent()->AdjustCurrentDopingGage(DopingAmount);

	AdjustQuantity(-1);
}
