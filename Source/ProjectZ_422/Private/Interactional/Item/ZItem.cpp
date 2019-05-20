// Fill out your copyright notice in the Description page of Project Settings.


#include "ZItem.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPickup.h"
#include "ZGameInstance.h"
#include "Engine/World.h"


// Sets default values
AZItem::AZItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bCanDestroy = true;
	bIsActive = true;
	ItemName = TEXT("Default");
	MaxQuantityOfItem = 999;
	CurrentQuantityOfItem = 1;
	ItemWeight = 0;
	InventoryIndex = -1;
	ItemOwner = nullptr;
	ItemType = EItemType::Default;
	Pickup = nullptr;
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

	AdjustQuantity(-1);

}

void AZItem::OnRemoved()
{
	ZLOG_S(Warning);

	//SetItemOwner(nullptr);
	SetCurrentQuantityOfItem(1);
	SetInventoryIndex(-1);

	OnItemRemoved.Broadcast();

	if (IsCanDestroy())
	{
		Destroy();
	}
	else
	{
		SetActive(false);
	}
}

void AZItem::OnDropped()
{
	ZLOG_S(Warning);

	/*
		Pickup Spawn���� ����
	*/
	FVector SpawnLocation;
	FHitResult Hit = ItemOwner->GetTraceHitFromActorCameraView(150.f);
	if (Hit.bBlockingHit)
	{
		SpawnLocation = Hit.ImpactPoint;
	}
	else
	{
		SpawnLocation = Hit.TraceEnd;
	}

	
	if (Pickup)
	{
		/*
			���� Pickup���� ������ Item�̶��
		*/
		// Pickup Ȱ��ȭ
		ZLOG(Warning, TEXT("Spawn actor."));
		Pickup->SetActive(true);
		Pickup->SetActorLocation(SpawnLocation);
		Pickup->WhenSpawnedByItem();
	}
	else
	{
		/*
			���� Pickup���� ������ Item�� �ƴ϶�� ex) �������� ������ ���
		*/
		// Pickup ����


	}

	// ItemStatusComponent���� �ش� Item����
	ItemOwner->GetItemStatusComponent()->RemoveItem(GetInventoryIndex(), true);

	//// Set ItemOwner null
	//SetItemOwner(nullptr);

	OnItemRemoved.Broadcast();

	SetActive(false);
}

int32 AZItem::AdjustQuantity(int32 Value) 
{
	int32 Quantity = CurrentQuantityOfItem + Value;
	int32 Remain = 0; // Item�� �ִ� ���������� �ʰ��� ��
	if (Quantity > MaxQuantityOfItem)
	{
		CurrentQuantityOfItem = MaxQuantityOfItem;
		Remain = Quantity - CurrentQuantityOfItem;
	}
	else
	{
		CurrentQuantityOfItem = FMath::Clamp<int32>(Quantity, 0, Quantity);
	}

	OnItemInfoChanged.Broadcast();

	// Item�� ���� �����ߴ��� �˻�.
	CheckItemExhausted();

	return Remain;
}

void AZItem::SetCanDestroy(bool NewState)
{
	bCanDestroy = NewState;
}

void AZItem::SetItemName(const FString & NewItemName)
{
	ItemName = NewItemName;

	OnItemInfoChanged.Broadcast();
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

	OnItemInfoChanged.Broadcast();
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

void AZItem::SetPickup(AZPickup * NewPickup)
{
	if (nullptr == NewPickup)
	{
		ZLOG(Error, TEXT("Invalid value."));
	}

	Pickup = NewPickup;
}

void AZItem::SetActive(bool NewState)
{
	if (NewState)
	{
		SetActorEnableCollision(ECollisionEnabled::QueryOnly);
		SetActorHiddenInGame(false);
	}
	else
	{
		SetActorEnableCollision(ECollisionEnabled::NoCollision);
		SetActorHiddenInGame(true);
	}

	bIsActive = NewState;
}


bool AZItem::IsCanDestroy() const
{
	return bCanDestroy;
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

bool AZItem::IsActive() const
{
	return bIsActive;
}

void AZItem::CheckItemExhausted()
{
	if (GetCurrentQuantityOfItem() < 1)
	{
		ItemOwner->GetItemStatusComponent()->RemoveItem(GetInventoryIndex());
	}
}

