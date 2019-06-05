// Fill out your copyright notice in the Description page of Project Settings.


#include "ZDoping.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"

AZDoping::AZDoping()
{

	ItemType = EItemType::Doping;

	// Code to test
	ItemName = TEXT("Doping");
}

void AZDoping::InitItemData(const FZItemData * NewItemData)
{
	Super::InitItemData(NewItemData);

	auto NewDopingData = static_cast<const FZDopingData*>(NewItemData);

	DopingAmount = NewDopingData->DopingAmount;
	DopingTime = NewDopingData->DopingTime;

}

void AZDoping::OnUsed()
{
	ItemOwner->GetStatusComponent()->AdjustCurrentDopingGage(DopingAmount);

	Super::OnUsed();
}
