// Fill out your copyright notice in the Description page of Project Settings.


#include "ZItem.h"
#include "ZCharacter.h"

// Sets default values
AZItem::AZItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemName = TEXT("Default");
	MaxQuantityOfItem = 999;
	CurrentQuantityOfItem = 1;
	ItemWeight = 0;
	InventoryIndex = -1;
	ItemOwner = nullptr;
	ItemType = EItemType::Default;
}

// Called when the game starts or when spawned
void AZItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZItem::OnUsed()
{
	ZLOG(Warning, TEXT("Use Item!"));

}

int32 AZItem::AdjustQuantity(int32 Value) 
{
	int32 Quantity = CurrentQuantityOfItem + Value;
	int32 Remain = 0;
	if (Quantity > MaxQuantityOfItem)
	{
		CurrentQuantityOfItem = MaxQuantityOfItem;
		Remain = Quantity - CurrentQuantityOfItem;
	}
	else
	{
		CurrentQuantityOfItem = FMath::Clamp<int32>(Quantity, 0, Quantity);
	}

	return Remain;
}

void AZItem::SetItemName(const FString & NewItemName)
{
	ItemName = NewItemName;
}

void AZItem::SetMaxQuantityOfItem(int32 NewValue)
{
	if (NewValue < 1)
	{
		return;
	}

	MaxQuantityOfItem = NewValue;
}

void AZItem::SetCurrentQuantityOfItem(int32 NewValue)
{
	if (!FMath::IsWithinInclusive<int32>(NewValue, 0, MaxQuantityOfItem))
	{
		return;
	}

	CurrentQuantityOfItem = NewValue;

}

void AZItem::SetItemWeight(int32 NewWeight)
{
	if (NewWeight < 0)
	{
		return;
	}
	
	ItemWeight = NewWeight;
}

void AZItem::SetInventoryIndex(int32 NewIndex)
{
	if (NewIndex < -1)
	{
		return;
	}

	InventoryIndex = NewIndex;
}

void AZItem::SetItemOwner(AZCharacter * NewItemOwner)
{
	ItemOwner = NewItemOwner;
}

const FString & AZItem::GetItemName() const
{
	return ItemName;
}

int32 AZItem::GetMaxQuantityOfItem() const
{
	return MaxQuantityOfItem;
}

int32 AZItem::GetCurrentQuantityOfItem() const
{
	return CurrentQuantityOfItem;
}

int32 AZItem::GetItemWeight() const
{
	return ItemWeight;
}

int32 AZItem::GetInventoryIndex() const
{
	return InventoryIndex;
}

AZCharacter * const AZItem::GetItemOwner() const
{
	return ItemOwner;
}

bool AZItem::IsItemQuantityMaximum() const
{
	return CurrentQuantityOfItem == MaxQuantityOfItem;
}

EItemType AZItem::GetItemType() const
{
	return ItemType;
}

