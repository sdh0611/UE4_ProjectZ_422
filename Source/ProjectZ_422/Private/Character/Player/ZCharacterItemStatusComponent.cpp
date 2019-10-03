// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterItemStatusComponent.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZInventoryWidget.h"
#include "ZWeaponInventoryWidget.h"
#include "ZShopWidget.h"
#include "ZPickup.h"
#include "ZWeapon.h"
#include "ZGun.h"
#include "ZRecovery.h"
#include "ZDoping.h"
#include "ZGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UZCharacterItemStatusComponent::UZCharacterItemStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bReplicates = true;

	// ...
	MaxSizeOfItemList = 30;
	CurrentSizeOfItemList = 0;
	MaxWeight = 100;
	CurrentWeight = 0;
	CurrentMoney = 1000;
	OwnerCharacter = nullptr;
}


// Called when the game starts
void UZCharacterItemStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ItemList �ʱ�ȭ
	ItemList.Init(nullptr, MaxSizeOfItemList);

	// WeaponInventory �ʱ�ȭ
	WeaponInventory.Init(nullptr, 4);

	OwnerCharacter = Cast<AZCharacter>(GetOwner());
}


// Called every frame
void UZCharacterItemStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UZCharacterItemStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UZCharacterItemStatusComponent, ItemList);
	DOREPLIFETIME(UZCharacterItemStatusComponent, WeaponInventory);
	DOREPLIFETIME(UZCharacterItemStatusComponent, CurrentSizeOfItemList);
	DOREPLIFETIME(UZCharacterItemStatusComponent, CurrentWeight);
	DOREPLIFETIME(UZCharacterItemStatusComponent, CurrentMoney);

}

void UZCharacterItemStatusComponent::AddItem(AZItem * NewItem)
{
	/*
		TODO(5.19) : EquipWeapon�� OnDropped�� �����ϴ� �κп��� ���׹߻�.
						-> ItemOwner�� null�ε� ItemOwner�� �ҷ��� ���� ����. �̰� ��ġ��
	*/
	UKismetSystemLibrary::PrintString(GetWorld(), NewItem->GetItemName());
	ZLOG(Warning, TEXT("%s"), *NewItem->GetItemName());

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
			NewItem->SetItemOwner(OwnerCharacter.Get());
			NewItem->MulticastOnItemPicked();

			auto PlayerController = Cast<AZPlayerController>(OwnerCharacter->GetController());
			if (PlayerController && PlayerController->IsLocalPlayerController())
			{
				PlayerController->AddItemToInventoryWidget(NewItem);
				PlayerController->AddItemToSellWidget(NewItem);
			}

		}

	}
	else
	{
		ZLOG(Warning, TEXT("Item already exist."));
		/*
			���� Item�� ItemList���� �ִ� ���
		*/
		// �ش� Item�� ������ ����������.
		int32 Remain = Item->AdjustQuantity(NewItem->GetCurrentQuantityOfItem());

		if (Remain > 0)
		{
			// �ش� Item�� �ִ� ���� ������ �ʰ����� ��
			// AddItem() ���ȣ��
			NewItem->SetCurrentQuantityOfItem(Remain);
			AddItem(NewItem);
		}
		else
		{
			// �ش� Item�� �ִ� ������ �ʰ����� ���� ���
			NewItem->Destroy();
		}

	}

	if (::IsValid(NewItem))
	{
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
	}
}

void UZCharacterItemStatusComponent::AddItem(class AZPickup* NewPickup)
{
	if (ROLE_Authority != GetOwnerRole())
	{
		ServerAddItem(NewPickup);
		return;
	}

	if (nullptr == NewPickup)
	{
		return;
	}

	//UKismetSystemLibrary::PrintString(GetWorld(), NewPickup->GetItemInfo().ItemName);
	//ZLOG(Warning, TEXT("Pickup item name : %s"), *NewPickup->GetItemInfo().ItemName);

	if (::IsValid(NewPickup) && NewPickup->GetItem())
	{
		AddItem(NewPickup->GetItem());
	}

	//auto Item = GetItemByName(NewPickup->GetItem()->GetItemName());
	//if (nullptr == Item || Item->IsItemQuantityMaximum())
	//{
	//	/*
	//		Item�� ItemList���� ���� ���
	//		or �ش�Item�� �ִ� �������� �����ϰ� ���� ��
	//	*/
	//	// ItemList ���� �� ������ �־���.
	//	int32 AllocatedIndex = AllocateInventoryIndex();
	//	if (AllocatedIndex == -1)
	//	{
	//		/*
	//			ItemList���� �� ������ ���� ���.
	//			 -> �ƹ��͵� �������� �ʰ� ����.
	//		*/
	//		return;
	//	}

	//	/*
	//		ItemList���� �Ҵ��� �� �ִ� ������ �ִ� ���
	//	*/
	//	// �ش� Index�� Item������ �Ҵ�.
	//	FActorSpawnParameters SpawnParams;
	//	SpawnParams.Owner = GetOwner();
	//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//	AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(NewPickup->SpawnItemClass, SpawnParams);
	//	if (nullptr == NewItem)
	//	{
	//		ZLOG(Error, TEXT("Failed to spawn item.."));
	//		return;
	//	}
	//	
	//	auto ItemData = GetWorld()->GetGameInstance<UZGameInstance>()->GetItemDataByName(NewPickup->GetItemInfo().ItemName);
	//	if (nullptr == ItemData)
	//	{
	//		ZLOG(Error, TEXT("Failed to find item data.."));
	//		NewItem->Destroy();
	//		return;
	//	}

	//	NewItem->InitItemData(ItemData);
	//	ZLOG(Error, TEXT("Pickup"));
	//	NewItem->ApplyItemInfo(NewPickup->GetItemInfo());
	//	NewItem->SetInventoryIndex(AllocatedIndex);
	//	NewItem->SetItemOwner(OwnerCharacter.Get());

	//	ItemList[AllocatedIndex] = NewItem;

	//	if (EItemType::Weapon == NewItem->GetItemType())
	//	{
	//		EquipWeapon(Cast<AZWeapon>(NewItem));
	//	}

	//	auto PlayerController = Cast<AZPlayerController>(OwnerCharacter->GetController());
	//	if (PlayerController && PlayerController->IsLocalPlayerController())
	//	{
	//		PlayerController->AddItemToInventoryWidget(NewItem);
	//		PlayerController->AddItemToSellWidget(NewItem);
	//	}

	//}
	//else
	//{
	//	ZLOG(Warning, TEXT("Item already exist."));
	//	/*
	//		���� Item�� ItemList���� �ִ� ���
	//	*/

	//	// �ش� Item�� ������ ����������.
	//	int32 Remain = Item->AdjustQuantity(NewPickup->GetItemInfo().CurrentQuantityOfItem);
	//	if (Remain > 0)
	//	{
	//		// �ش� Item�� �ִ� ���� ������ �ʰ����� ��
	//		FActorSpawnParameters SpawnParams;
	//		SpawnParams.Owner = GetOwner();
	//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//		AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(NewPickup->SpawnItemClass, SpawnParams);
	//		if (nullptr == NewItem)
	//		{
	//			ZLOG(Error, TEXT("Failed to spawn item.."));
	//			return;
	//		}
	//		auto ItemData = GetWorld()->GetGameInstance<UZGameInstance>()->GetItemDataByName(NewPickup->GetItemInfo().ItemName);
	//		if (nullptr == ItemData)
	//		{
	//			ZLOG(Error, TEXT("Failed to find item data.."));
	//			NewItem->Destroy();
	//			return;
	//		}

	//		NewItem->InitItemData(ItemData);
	//		NewItem->SetCurrentQuantityOfItem(Remain);

	//		if (EItemType::Weapon == NewItem->GetItemType())
	//		{
	//			EquipWeapon(Cast<AZWeapon>(NewItem));
	//		}
	//		// �ش� Item�� �ٽ� �־���.
	//		AddItem(NewItem);
	//	}

	//}

	/* Pickup ���� */
	NewPickup->Destroy();

}

class AZWeapon* const UZCharacterItemStatusComponent::EquipWeapon(AZWeapon * NewWeapon)
{
	ZLOG_S(Warning);
	/*
		�׽�Ʈ�� ���� �ӽ÷� CurrentWeapon ������ ������ WeaponInventory�� ù��° �ε����� �ο���.
	*/

	/*
		�Ǽ��̶�� ���´� ���Ⱑ ���� ��츸�̶�� ����.
	*/

	switch (NewWeapon->GetWeaponCategory())
	{
		case EWeaponCategory::Gun:
		{
			if (WeaponInventory[Main1])
			{
				/*
					�ֹ��� ����(ù��°, �ι�°) �� ù��° ���Կ� �̹� �ֹ��Ⱑ �ִ� ���.
				*/

				/*
					TODO(5.18) : �� �κп��� Inventory Widget�� ������ �Ͼ�� ����.
									Item�� OnDropped ��������Ʈ�� �����ϴ��� �ؾ���
									-> OnItemRemoved ��������Ʈ�� �����Ŵ.
				*/

				if (WeaponInventory[Main2])
				{
					/*
						�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ��� ���Ⱑ �ִ� ���.
					*/

					if (WeaponInventory[Main1]->IsEquipped())
					{
						/*
							ù��° ���� ���� ��ü
						*/
						//Dropped ���ο��� Equip���� ����
						WeaponInventory[Main1]->OnDropped();
						WeaponInventory[Main1] = NewWeapon;
						NewWeapon->SetWeaponInventoryIndex(Main1);
					}
					else
					{
						/*
							�ι��� ���� ���� ��ü
						*/
						//Dropped ���ο��� Equip���� ����
						WeaponInventory[Main2]->OnDropped();
						WeaponInventory[Main2] = NewWeapon;
						NewWeapon->SetWeaponInventoryIndex(Main2);
					}

					// Weapon �������� ����
					OwnerCharacter->SetCurrentWeapon(NewWeapon);

					// Main weapon socket�� ����
					auto Gun = Cast<AZGun>(NewWeapon);
					check(nullptr != Gun);

					FName SocketName;
					switch (Gun->GetGunType())
					{
						case EGunType::Shotgun:
						{
							SocketName = OwnerCharacter->GetMainWeaponShotgunSocketName();
							break;
						}
						default:
						{
							SocketName = OwnerCharacter->GetMainWeaponSocketName();
							break;
						}
					}
					NewWeapon->AttachSocketName = SocketName;
					OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);

				}
				else
				{
					/*
						�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ� ���Ⱑ ���� ���.
					*/

					// 2��° ���Կ� Weapon ����
					WeaponInventory[Main2] = NewWeapon;
					NewWeapon->SetWeaponInventoryIndex(Main2);

					// Secondary Weapon ���Ͽ� Weapon ����
					NewWeapon->AttachSocketName = OwnerCharacter->GetThirdWeaponSocketName();
					OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);

				}


			}
			else
			{
				/*
					�ֹ��� ����(ù��°, �ι�°) �� ù��° ���Կ� �ֹ��Ⱑ ���� ���.
				*/
				// 0��° ���Կ� Weapon ����
				WeaponInventory[Main1] = NewWeapon;
				NewWeapon->SetWeaponInventoryIndex(Main1);

				if (WeaponInventory[Main2])
				{
					/*
						�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ� ���Ⱑ �ִ� ���.
					*/
					if (OwnerCharacter->IsEquipWeapon())
					{
						// Secondary Weapon ���Ͽ� Weapon ����
						NewWeapon->AttachSocketName = OwnerCharacter->GetSecondaryWeaponSocketName();
						OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);

					}
					else
					{
						OwnerCharacter->SetCurrentWeapon(NewWeapon);
						// Main Weapon ���Ͽ� Weapon ����
						auto Gun = Cast<AZGun>(NewWeapon);
						check(nullptr != Gun);

						FName SocketName;
						switch (Gun->GetGunType())
						{
							case EGunType::Shotgun:
							{
								SocketName = OwnerCharacter->GetMainWeaponShotgunSocketName();
								break;
							}
							default:
							{
								SocketName = OwnerCharacter->GetMainWeaponSocketName();
								break;
							}
						}
						NewWeapon->AttachSocketName = SocketName;
						OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);

					}

				}
				else
				{
					/*
						�ֹ��� ����(ù��°, �ι�°) �� �ι�° ���Կ��� ���Ⱑ ���� ���.
					*/
					OwnerCharacter->SetCurrentWeapon(NewWeapon);
					// Main Weapon ���Ͽ� Weapon ����
					auto Gun = Cast<AZGun>(NewWeapon);
					check(nullptr != Gun);

					FName SocketName;
					switch (Gun->GetGunType())
					{
						case EGunType::Shotgun:
						{
							SocketName = OwnerCharacter->GetMainWeaponShotgunSocketName();
							break;
						}
						default:
						{
							SocketName = OwnerCharacter->GetMainWeaponSocketName();
							break;
						}
					}
					NewWeapon->AttachSocketName = SocketName;
					OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);

				}

			}
			break;
		}
		case EWeaponCategory::Knife:
		{


			break;
		}
		case EWeaponCategory::Grenade:
		{
			if (nullptr == WeaponInventory[Grenade])
			{
				/*
					Grenade�� �������� ���� ���
				*/
				// Grenade ���Կ� �߰�
				WeaponInventory[Grenade] = NewWeapon;
				NewWeapon->SetWeaponInventoryIndex(Grenade);
				// Grenade socket�� mesh ����.
				NewWeapon->AttachSocketName = OwnerCharacter->GetGrenadeWeaponSocketName();
				OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);
			}

			break;
		}
		default:
		{
			ZLOG(Error, TEXT("Invalid category."));
			return nullptr;
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
		if (Weapon->GetWeaponInventoryIndex() != EWeaponSlot::Invalid)
		{
			WeaponInventory[Weapon->GetWeaponInventoryIndex()] = nullptr;
		}

		Weapon->SetWeaponInventoryIndex(EWeaponSlot::Invalid);
	}

	ItemList[InventoryIndex]->SetInventoryIndex(EWeaponSlot::Invalid);

	if (!bIsDropped)
	{
		/*
			Remove�� ���
		*/
		// �ش� Item�� Remove �޼ҵ� ȣ��.
		ItemList[InventoryIndex]->OnRemoved();
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
	auto PlayerController = Cast<AZPlayerController>(OwnerCharacter->GetController());
	if (nullptr == PlayerController)
	{
		return;
	}

	OnRep_CurrentMoney();
}

void UZCharacterItemStatusComponent::RemoveAllItem()
{
	for (const auto& Item : ItemList)
	{
		Item->OnRemoved();
	}
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
		TempItemList[Index]->OnDropped();
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

bool UZCharacterItemStatusComponent::ServerAddItem_Validate(class AZPickup* NewPickup)
{
	return true;
}

void UZCharacterItemStatusComponent::ServerAddItem_Implementation(class AZPickup* NewPickup)
{
	AddItem(NewPickup);
}


void UZCharacterItemStatusComponent::OnRep_CurrentMoney()
{
	OnMoneyInfoChange.Broadcast(CurrentMoney);
}

AZItem * UZCharacterItemStatusComponent::GetItemByIndex(int32 ItemIndex) const
{
	if (!FMath::IsWithinInclusive<int32>(ItemIndex, 0, MaxSizeOfItemList))
	{
		return nullptr;
	}

	return ItemList[ItemIndex];
}

AZItem * UZCharacterItemStatusComponent::GetItemByName(const FString & ItemName)
{
	AZItem* Result = nullptr;

	for (const auto& Item : ItemList)
	{
		if (Item)
		{
			if (ItemName == Item->GetItemName())
			{
				Result = Item;
				if (!Item->IsItemQuantityMaximum())
				{
					break;
				}
			}
		}
	}

	return Result;
}

