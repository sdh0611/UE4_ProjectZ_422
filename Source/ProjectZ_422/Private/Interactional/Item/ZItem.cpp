// Fill out your copyright notice in the Description page of Project Settings.


#include "ZItem.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPickup.h"
#include "ZGameInstance.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"
#include "UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "ZUserHUD.h"
#include "ZPlayerController.h"
#include "ZPlayerAnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AZItem::AZItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	bCanDestroy = true;
	bIsActive = true;
	ItemName = TEXT("Default");
	MaxQuantityOfItem = 999;
	CurrentQuantityOfItem = 1;
	ItemWeight = 0;
	InventoryIndex = -1;
	ItemOwner = nullptr;
	ItemType = EItemType::Default;

	/* Relicate�� ���� ���� ������Ʈ */
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	static ConstructorHelpers::FClassFinder<AZPickup>
		CLASS_PICKUP(TEXT("Blueprint'/Game/Blueprint/Game/Interactional/Pickup/BP_ZPickup.BP_ZPickup_C'"));
	if (CLASS_PICKUP.Succeeded())
	{
		PickupClass = CLASS_PICKUP.Class;
	}

	Tags.Add(TEXT("Item"));
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

void AZItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZItem, bCanDestroy);
	DOREPLIFETIME(AZItem, bIsActive);
	DOREPLIFETIME(AZItem, ItemName);
	DOREPLIFETIME(AZItem, MaxQuantityOfItem);
	DOREPLIFETIME(AZItem, CurrentQuantityOfItem);
	DOREPLIFETIME(AZItem, InventoryIndex);
	DOREPLIFETIME(AZItem, ItemExplanation);
	DOREPLIFETIME(AZItem, ItemOwner);
}

void AZItem::Destroyed()
{
	ZLOG_S(Error);
	OnItemRemoved.Broadcast();
	ClearDelegates();
	
	Super::Destroyed();
}

void AZItem::OnDropped(int32 Quantity)
{
	if (!HasAuthority())
	{
		ServerOnDropItem(Quantity);
		return;
	}

	ZLOG_S(Error);
	if (Quantity < 1)
	{
		return;
	}

	/*
		Pickup Spawn���� ����
	*/
	FVector SpawnLocation;
	if (::IsValid(ItemOwner) && ItemOwner->IsDead())
	{
		ZLOG_S(Error);
		SpawnLocation = ItemOwner->GetActorLocation();
		SpawnLocation.Z += 30.f;
	}
	else
	{
		FHitResult Hit = ItemOwner->GetTraceHitFromActorCameraView(150.f);
		if (Hit.bBlockingHit)
		{
			ZLOG_S(Error);
			SpawnLocation = Hit.ImpactPoint;
		}
		else
		{
			ZLOG_S(Error);
			SpawnLocation = Hit.TraceEnd;
		}
		ZLOG_S(Error);
	}


	if (GetCurrentQuantityOfItem() <= Quantity)
	{
		ZLOG(Error, TEXT("Drop all"));
		{
			/*
				���� Pickup���� ������ Item�� �ƴ϶�� ex) �������� ������ ���
			*/
			// Pickup ����
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = GetOwner();

			AZPickup* NewPickup = GetWorld()->SpawnActor<AZPickup>(PickupClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			if (nullptr == NewPickup)
			{
				ZLOG(Error, TEXT("Failed to spawn pickup...."));
				return;
			}

			NewPickup->SpawnItemClass = GetClass();
			NewPickup->SetItem(this);
			NewPickup->WhenSpawnedByItem();

		}

		ClientOnItemRemoved();
		if (HasAuthority())
		{
			OnItemRemoved.Broadcast();
			ClearDelegates();
		}
		
		ItemOwner->GetItemStatusComponent()->RemoveItem(GetInventoryIndex(), true);
		MulticastOnItemDropped();
	}
	else
	{
		ZLOG(Error, TEXT("Drop %d"), Quantity);
		/* �ʱ�ȭ�� ����� ItemData �˻�. */
		auto MyGameInstance = GetGameInstance<UZGameInstance>();
		check(MyGameInstance);

		/* ���ο� Item spawn */
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = GetOwner();

		AZPickup* NewPickup = GetWorld()->SpawnActor<AZPickup>(PickupClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (nullptr == NewPickup)
		{
			ZLOG(Error, TEXT("Failed to spawn pickup.."));
			return;
		}

		AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (nullptr == NewItem)
		{
			ZLOG(Error, TEXT("Failed to spawn new item.."));
			NewPickup->Destroy();
			return;
		}

		auto ItemData = MyGameInstance->GetItemDataByName(ItemName);
		if (nullptr == ItemData)
		{
			ZLOG(Error, TEXT("Failed to find data : %s.."), *ItemName);
			NewPickup->Destroy();
			return;
		}

		NewItem->InitItemData(ItemData);
		NewItem->SetCurrentQuantityOfItem(Quantity);

		NewPickup->SetItem(NewItem);
		NewPickup->WhenSpawnedByItem();

		AdjustQuantity(-Quantity);
	}


}

void AZItem::OnRemoved()
{
	ZLOG_S(Warning);

	//ClientOnItemRemoved();

	if (IsCanDestroy())
	{
		Destroy();
	}
	else
	{
		//SetItemOwner(nullptr);
		ClearDelegates();
		SetCurrentQuantityOfItem(1);
		SetInventoryIndex(-1);
		SetActive(false);
	}
}

void AZItem::OnPicked()
{
}

void AZItem::WhenDropped()
{
}

void AZItem::InitItemData(const FZItemData * const NewItemData)
{
	if (!HasAuthority())
	{
		return;
	}

	/*
		Data ��ȿ�� üũ
	*/
	if (nullptr == NewItemData)
	{
		ZLOG(Error, TEXT("Invalid data."));
		return;
	}

	ItemName = NewItemData->ItemName;
	ItemWeight = NewItemData->ItemWeight;
	MaxQuantityOfItem = NewItemData->MaxQuantity;
	ItemExplanation = NewItemData->ItemExplanation;

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

	//OnItemInfoChanged.Broadcast();
	if (HasAuthority())
	{
		OnRep_ItemInfoChanged();
	}
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
	if (!HasAuthority())
	{
		return;
	}
	ItemName = NewItemName;

	//OnItemInfoChanged.Broadcast();
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
	if (!HasAuthority())
	{
		return;
	}

	if (!FMath::IsWithinInclusive<int32>(NewValue, 0, MaxQuantityOfItem))
	{
		return;
	}

	CurrentQuantityOfItem = NewValue;

	//OnItemInfoChanged.Broadcast();
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
	if (HasAuthority())
	{
		if (::IsValid(NewItemOwner))
		{
			auto MyPC = NewItemOwner->GetController<AZPlayerController>();
			if (MyPC)
			{
				SetOwner(MyPC);
			}
		}
		else
		{
			SetOwner(nullptr);
		}
	}

	ItemOwner = NewItemOwner;
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

const FText & AZItem::GetItemExplanation() const
{
	return ItemExplanation;
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

UAnimMontage * const AZItem::FindMontage(const FString & MontageName) const
{
	if (MontageTable.Contains(MontageName))
	{
		return MontageTable[MontageName];
	}

	return nullptr;
}

void AZItem::RepItemOwner()
{
	if (ItemOwner)
	{
		auto MyPC = ItemOwner->GetController<AZPlayerController>();
		if (MyPC)
		{
			MyPC->AddItemToInventoryWidget(this);
			MyPC->AddItemToSellWidget(this);
		}
	}

}

void AZItem::CheckItemExhausted()
{
	if (!HasAuthority())
	{
		return;
	}

	if (GetCurrentQuantityOfItem() < 1)
	{
		ItemOwner->GetItemStatusComponent()->RemoveItem(GetInventoryIndex());
	}
}

void AZItem::ClearDelegates()
{
	OnItemInfoChanged.Clear();
	OnItemRemoved.Clear();
}

bool AZItem::ServerOnDropItem_Validate(int32 Quantity)
{
	return true;
}

void AZItem::ServerOnDropItem_Implementation(int32 Quantity)
{
	OnDropped(Quantity);
}

bool AZItem::ClientOnItemRemoved_Validate()
{
	return true;
}

void AZItem::ClientOnItemRemoved_Implementation()
{
	ZLOG_S(Error);
	OnItemRemoved.Broadcast();
	ClearDelegates();
}

void AZItem::MulticastOnItemDropped_Implementation()
{
	WhenDropped();
}

void AZItem::MulticastOnItemPicked_Implementation()
{
	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		return;
	}

	OnPicked();
}

void AZItem::OnRep_ItemOwner()
{
	RepItemOwner();
}

void AZItem::OnRep_ItemInfoChanged()
{
	OnItemInfoChanged.Broadcast();
}

