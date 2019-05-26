// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterItemStatusComponent.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZInventoryWidget.h"
#include "ZShopWidget.h"
#include "ZPickup.h"
#include "ZWeapon.h"
#include "ZRecovery.h"
#include "ZDoping.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values for this component's properties
UZCharacterItemStatusComponent::UZCharacterItemStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	MaxSizeOfItemList = 30;
	CurrentSizeOfItemList = 0;
	MaxWeight = 100;
	CurrentWeight = 0;
	CurrentMoney = 0;
	OwnerCharacter = nullptr;
}


// Called when the game starts
void UZCharacterItemStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ItemList �ʱ�ȭ
	ItemList.Init(nullptr, MaxSizeOfItemList);

	// WeaponInventory �ʱ�ȭ
	WeaponInventory.Init(nullptr, 5);

	OwnerCharacter = Cast<AZCharacter>(GetOwner());
}


// Called every frame
void UZCharacterItemStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UZCharacterItemStatusComponent::AddItem(AZItem * NewItem, AZPickup* OwnerPickup)
{
	/*
		TODO(5.19) : EquipWeapon�� OnDropped�� �����ϴ� �κп��� ���׹߻�.
						-> ItemOwner�� null�ε� ItemOwner�� �ҷ��� ���� ����. �̰� ��ġ��
	*/

	ZLOG(Warning, TEXT("%s"), *NewItem->GetItemName());

	switch (NewItem->GetItemType())
	{
		case EItemType::Weapon:
		{
			ZLOG(Warning, TEXT("Weapon!!"));
			
			// Code to test
			EquipWeapon(Cast<AZWeapon>(NewItem));
			break;
		}
		case EItemType::Recovery:
		{
			ZLOG(Warning, TEXT("Recovery!!"));
			break;
		}
		case EItemType::Doping:
		{
			ZLOG(Warning, TEXT("Doping!!"));
			break;
		}
		default:
		{
			ZLOG(Warning, TEXT("No type!!"));
			break;
		}
	}

	auto Item = GetItemByName(NewItem->GetItemName());
	if (nullptr == Item || Item->IsItemQuantityMaximum())
	{
		/*
			Item�� ItemList���� ���� ��� 
			or �ش�Item�� �ִ� �������� �����ϰ� ���� ��
		*/
		// ItemList ���� �� ������ �־���.
		int32 AllocatedIndex = AllocateInventoryIndex();
		if (AllocatedIndex == -1)
		{
			/* 
				ItemList���� �� ������ ���� ���.
			*/
			// �ٽ� Pickup Actor�� ����� Drop��Ŵ. -> ���� Item Drop�ڵ� ������ ��.

		}
		else
		{
			/* 
				ItemList���� �Ҵ��� �� �ִ� ������ �ִ� ��� 
			*/
			// �ش� Index�� Item������ �Ҵ�.
			ItemList[AllocatedIndex] = NewItem;
			NewItem->SetInventoryIndex(AllocatedIndex);
			NewItem->SetItemOwner(OwnerCharacter);
			if (OwnerPickup)
			{
				NewItem->SetPickup(OwnerPickup);
				OwnerPickup->SetItem(NewItem);
			}

			auto PlayerController = Cast<AZPlayerController>(OwnerCharacter->GetController());
			if (PlayerController)
			{
				// Inventory�� Update
				auto InventoryWidget = PlayerController->GetZHUD()->GetUserHUD()->GetInventoryWidget();
				if (InventoryWidget)
				{
					InventoryWidget->AddItemToInventory(NewItem);
				}

				// ShopSellWidget Update
				if (PlayerController->GetZHUD()->GetUserHUD()->IsShopWidgetOnScreen())
				{
					ZLOG(Warning, TEXT("Add Item to SellWidget"));
					auto ShopSellWidget = PlayerController->GetZHUD()->GetUserHUD()->GetShopWidget();
					if (ShopSellWidget)
					{
						ShopSellWidget->AddItemToSellWidget(NewItem);
					}
				}
			}

		}

	}
	else
	{
		ZLOG(Warning, TEXT("Item already exist."));
		/* 
			���� Item�� ItemList���� �ִ� ���
		*/
		// OwnerPickup ����
		if (OwnerPickup)
		{
			OwnerPickup->Destroy();
		}

		// �ش� Item�� ������ ����������.
		int32 Remain = Item->AdjustQuantity(NewItem->GetCurrentQuantityOfItem());

		if (Remain > 0)
		{
			// �ش� Item�� �ִ� ���� ������ �ʰ����� ��
			// AddItem() ���ȣ��
			AddItem(NewItem);
		}
		else
		{
			// �ش� Item�� �ִ� ������ �ʰ����� ���� ���
			NewItem->Destroy();
		}

	}



}

class AZWeapon* const UZCharacterItemStatusComponent::EquipWeapon(AZWeapon * NewWeapon)
{
	ZLOG_S(Warning);
	/*
		�׽�Ʈ�� ���� �ӽ÷� CurrentWeapon ������ ������ WeaponInventory�� ù��° �ε����� �ο���.
	*/
	
	/*
		�Ǽ��̶�� ���´� �ֹ��� 2���� ���� ��츸�̶�� ����.
	*/

	if (WeaponInventory[0])
	{
		/*
			�ֹ��� ����(ù��°, �ι�°) �� ù��° ���Կ� �̹� �ֹ��Ⱑ �ִ� ���.
		*/

		/*
			TODO(5.18) : �� �κп��� Inventory Widget�� ������ �Ͼ�� ����.
							Item�� OnDropped ��������Ʈ�� �����ϴ��� �ؾ���
							-> OnItemRemoved ��������Ʈ�� �����Ŵ.
		*/
				
		if (WeaponInventory[1])
		{
			/*
				�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ��� ���Ⱑ �ִ� ���.
			*/

			if (WeaponInventory[0]->IsEquipped())
			{
				/*
					ù��° ���� ���� ��ü
				*/
				//Dropped ���ο��� Equip���� ����
				WeaponInventory[0]->OnDropped();
				WeaponInventory[0] = NewWeapon;
				NewWeapon->SetWeaponInventoryIndex(0);
			}
			else
			{
				/*
					�ι��� ���� ���� ��ü
				*/
				//Dropped ���ο��� Equip���� ����
				WeaponInventory[1]->OnDropped();
				WeaponInventory[1] = NewWeapon;
				NewWeapon->SetWeaponInventoryIndex(1);
			}

			// Weapon �������� ����
			OwnerCharacter->SetCurrentWeapon(NewWeapon);
			// Main weapon socket�� ����
			NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AZCharacter::MainWeaponSocketName);
		}
		else
		{
			/*
				�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ� ���Ⱑ ���� ���.
			*/
		
			// 2��° ���Կ� Weapon ����
			WeaponInventory[1] = NewWeapon;
			NewWeapon->SetWeaponInventoryIndex(1);
			// Secondary Weapon ���Ͽ� Weapon ����
			NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AZCharacter::SecondaryWeaponSocketName);			
		}


	}
	else
	{
		/*
			�ֹ��� ����(ù��°, �ι�°) �� ù��° ���Կ� �̹� �ֹ��Ⱑ ���� ���.
		*/
		// 0��° ���Կ� Weapon ����
		WeaponInventory[0] = NewWeapon;
		NewWeapon->SetWeaponInventoryIndex(0);

		if (WeaponInventory[1])
		{
			/*
				�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ� ���Ⱑ �ִ� ���.
			*/
			if (OwnerCharacter->IsEquipWeapon())
			{
				// Secondary Weapon ���Ͽ� Weapon ����
				NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AZCharacter::SecondaryWeaponSocketName);
			}
			else
			{
				OwnerCharacter->SetCurrentWeapon(NewWeapon);
				// Main Weapon ���Ͽ� Weapon ����
				NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AZCharacter::MainWeaponSocketName);
			}

		}
		else
		{
			/*
				�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ��� ���Ⱑ ���� ���.
			*/
			OwnerCharacter->SetCurrentWeapon(NewWeapon);
			// Main Weapon ���Ͽ� Weapon ����
			NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AZCharacter::MainWeaponSocketName);
		}

	}

	return nullptr;
}

void UZCharacterItemStatusComponent::RemoveItem(int32 InventoryIndex, bool bIsDropped)
{
	ZLOG_S(Warning);
	// �ùٸ� Index������ Ȯ��
	if (!ItemList.IsValidIndex(InventoryIndex))
	{
		return;
	}

	// �ش� InventoryIndex�� Item�����Ͱ� �ִ��� Ȯ��.
	if (nullptr == ItemList[InventoryIndex])
	{
		return;
	}

	ItemList[InventoryIndex]->SetItemOwner(nullptr);

	// Weapon�� ��� �ش� WeaponInventory�� nullptr�� ����
	if (ItemList[InventoryIndex]->GetItemType() == EItemType::Weapon)
	{
		ZLOG(Warning, TEXT("Check weapon."));
		auto Weapon = Cast<AZWeapon>(ItemList[InventoryIndex]);
		if (Weapon->IsEquipped())
		{
			OwnerCharacter->SetCurrentWeapon(nullptr);
		}
		WeaponInventory[Weapon->GetWeaponInventoryIndex()] = nullptr;
		Weapon->SetWeaponInventoryIndex(-1);
	}

	if (!bIsDropped)
	{
		/*
			Remove�� ���
		*/
		// �ش� Item�� Remove �޼ҵ� ȣ��.
		ItemList[InventoryIndex]->OnRemoved();
	}
	else
	{
		ItemList[InventoryIndex]->SetInventoryIndex(-1);
	}
	   
	// �ش� Index�� Item �����͸� nullptr�� ����.
	ItemList[InventoryIndex] = nullptr;

}

void UZCharacterItemStatusComponent::DropWeapon(int32 WeaponInventoryIndex)
{
	if (!WeaponInventory.IsValidIndex(WeaponInventoryIndex))
	{
		return;
	}

	if (nullptr == WeaponInventory[WeaponInventoryIndex])
	{
		return;
	}


	WeaponInventory[WeaponInventoryIndex]->OnDropped();

}

AZWeapon * const UZCharacterItemStatusComponent::GetWeaponFromWeaponInventory(int32 NewWeaponIndex)
{
	if (!WeaponInventory.IsValidIndex(NewWeaponIndex))
	{
		return nullptr;
	}
	
	return WeaponInventory[NewWeaponIndex];
}

void UZCharacterItemStatusComponent::AdjustMoney(int32 Value)
{
	CurrentMoney += Value;
	if (CurrentMoney < 0)
	{
		CurrentMoney = 0;
	}
	ZLOG(Warning, TEXT("Current money : %d$"), CurrentMoney);
}

void UZCharacterItemStatusComponent::SetMaxSizeOfItemList(int32 NewMaxSize)
{
	if (!FMath::IsWithinInclusive<int32>(NewMaxSize, 0, CurrentSizeOfItemList))
	{
		return;
	}
	
	// ���� ItemList���� Item�����͵��� ���.
	TArray<AZItem*> TempItemList(ItemList);
	//ItemList ���Ҵ�
	ItemList.Init(nullptr, NewMaxSize);
	// Item�����͵��� ItemList�� �Ҵ�.
	for (int32 Index = 0; Index < NewMaxSize; ++Index)
	{
		ItemList[Index] = TempItemList[Index];
	}

	// ���� ItemList�� ���� ���� Item���� �ִٸ� ��� Drop
	for (int32 Index = ItemList.Num(); Index < MaxSizeOfItemList; ++Index)
	{
		//Item Drop �ڵ� ������ ��.

	}

	MaxSizeOfItemList = NewMaxSize;
}

void UZCharacterItemStatusComponent::SetCurrentSizeOfItemList(int32 NewCurrentSize)
{
	if (!FMath::IsWithinInclusive<int32>(NewCurrentSize, 0, MaxSizeOfItemList))
	{
		return;
	}

	CurrentSizeOfItemList = NewCurrentSize;
}

void UZCharacterItemStatusComponent::SetMaxWeight(int32 NewMaxWeight)
{
	if (NewMaxWeight < CurrentWeight)
	{
		return;
	}

	MaxWeight = NewMaxWeight;
}

void UZCharacterItemStatusComponent::SetCurrentWeight(int32 NewCurrentWeight)
{
	if (!FMath::IsWithinInclusive<int32>(NewCurrentWeight, 0, MaxWeight))
	{
		return;
	}

	CurrentWeight = NewCurrentWeight;
}

void UZCharacterItemStatusComponent::SetCurrentMoney(int32 NewCurrentMoney)
{
	if (NewCurrentMoney < 0)
	{
		return;
	}

	CurrentMoney = NewCurrentMoney;
}

int32 UZCharacterItemStatusComponent::GetMaxSizeOfItemList() const
{
	return MaxSizeOfItemList;
}

int32 UZCharacterItemStatusComponent::GetCurrentSizeOfItemList() const
{
	return CurrentSizeOfItemList;
}

int32 UZCharacterItemStatusComponent::GetMaxWeight() const
{
	return MaxWeight;
}

int32 UZCharacterItemStatusComponent::GetCurrentWeight() const
{
	return CurrentWeight;
}

int32 UZCharacterItemStatusComponent::GetCurrentMoney() const
{
	return CurrentMoney;
}

AZItem * UZCharacterItemStatusComponent::GetItemByIndex(int32 ItemIndex) const
{
	if (!FMath::IsWithinInclusive<int32>(ItemIndex, 0, MaxSizeOfItemList))
	{
		return nullptr;
	}

	return ItemList[ItemIndex];
}

bool UZCharacterItemStatusComponent::IsItemListFull() const
{
	return CurrentSizeOfItemList == MaxSizeOfItemList;
}

const TArray<class AZItem*>& UZCharacterItemStatusComponent::GetItemList() const
{
	return ItemList;
}

int32 UZCharacterItemStatusComponent::AllocateInventoryIndex()
{
	for (int32 Index = 0; Index < MaxSizeOfItemList; ++Index)
	{
		if (nullptr == ItemList[Index])
		{
			// ����ִ� ������ Index ��ȯ.
			return Index;
		}
	}

	// ItemList�� ���� ���.
	return -1;
}

AZItem * UZCharacterItemStatusComponent::GetItemByName(const FString & ItemName)
{
	for (const auto& Item : ItemList)
	{
		if (Item)
		{
			if (ItemName == Item->GetItemName() && !Item->IsItemQuantityMaximum())
			{
				return Item;
			}
		}
	}

	return nullptr;
}

