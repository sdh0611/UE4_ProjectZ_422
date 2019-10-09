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

	// ItemList 초기화
	ItemList.Init(nullptr, MaxSizeOfItemList);

	// WeaponInventory 초기화
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
		TODO(5.19) : EquipWeapon의 OnDropped를 실행하는 부분에서 버그발생.
						-> ItemOwner가 null인데 ItemOwner를 불러서 생긴 현상. 이거 고치자
	*/
	//UKismetSystemLibrary::PrintString(GetWorld(), NewItem->GetItemName());
	ZLOG(Warning, TEXT("%s"), *NewItem->GetItemName());

	auto Item = GetItemByName(NewItem->GetItemName());
	if (nullptr == Item || Item->IsItemQuantityMaximum())
	{
		/*
			Item이 ItemList내에 없는 경우
			or 해당Item이 최대 개수까지 보유하고 있을 때
		*/
		// ItemList 내의 빈 공간에 넣어줌.
		int32 AllocatedIndex = AllocateInventoryIndex();
		if (AllocatedIndex == -1)
		{
			/*
				ItemList내에 빈 공간이 없는 경우.
			*/
			// 다시 Pickup Actor를 만들어 Drop시킴. -> 후일 Item Drop코드 기재할 것.

		}
		else
		{
			/*
				ItemList내에 할당할 수 있는 공간이 있는 경우
			*/
			// 해당 Index에 Item포인터 할당.
			ItemList[AllocatedIndex] = NewItem;
			NewItem->SetInventoryIndex(AllocatedIndex);
			NewItem->SetItemOwner(OwnerCharacter.Get());
			NewItem->MulticastOnItemPicked();

			if (::IsValid(NewItem))
			{
				if (EItemType::Weapon == NewItem->GetItemType())
				{
					EquipWeapon(Cast<AZWeapon>(NewItem));
				}
			}

			auto PlayerController = Cast<AZPlayerController>(OwnerCharacter->GetController());
			if (PlayerController)
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
			같은 Item이 ItemList내에 있는 경우
		*/
		// 해당 Item의 개수를 증가시켜줌.
		int32 Remain = Item->AdjustQuantity(NewItem->GetCurrentQuantityOfItem());

		if (Remain > 0)
		{
			// 해당 Item의 최대 보유 개수를 초과했을 때
			// AddItem() 재귀호출
			NewItem->SetCurrentQuantityOfItem(Remain);
			AddItem(NewItem);
		}
		else
		{
			// 해당 Item의 최대 개수를 초과하지 않은 경우
			NewItem->Destroy();
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

	if (::IsValid(NewPickup) && NewPickup->GetItem())
	{
		AddItem(NewPickup->GetItem());

		/* Pickup 정리 */
		NewPickup->Destroy();
	}

}

class AZWeapon* const UZCharacterItemStatusComponent::EquipWeapon(AZWeapon * NewWeapon)
{
	if (ROLE_Authority != GetOwnerRole())
	{
		ServerEquipWeapon(NewWeapon);
	}
	ZLOG_S(Warning);
	/*
		테스트를 위해 임시로 CurrentWeapon 변수의 역할을 WeaponInventory의 첫번째 인덱스에 부여함.
	*/

	/*
		맨손이라는 상태는 무기가 없는 경우만이라고 가정.
	*/

	switch (NewWeapon->GetWeaponCategory())
	{
	case EWeaponCategory::Gun:
	{
		if (WeaponInventory[Main1])
		{
			/*
				주무기 슬롯(첫번째, 두번째) 중 첫번째 슬롯에 이미 주무기가 있는 경우.
			*/

			/*
				TODO(5.18) : 이 부분에서 Inventory Widget의 갱신이 일어나지 않음.
								Item에 OnDropped 델리게이트를 구현하던가 해야함
								-> OnItemRemoved 델리게이트를 실행시킴.
			*/

			if (WeaponInventory[Main2])
			{
				/*
					주무기 슬롯(첫번째, 두번째) 중 두번째 슬롯에도 무기가 있는 경우.
				*/

				if (WeaponInventory[Main1]->IsEquipped())
				{
					/*
						첫번째 슬롯 무기 교체
					*/
					//Dropped 내부에서 Equip여부 해제
					WeaponInventory[Main1]->OnDropped();
					WeaponInventory[Main1] = NewWeapon;
					NewWeapon->SetWeaponInventoryIndex(Main1);
				}
				else
				{
					/*
						두번쨰 슬롯 무기 교체
					*/
					//Dropped 내부에서 Equip여부 해제
					WeaponInventory[Main2]->OnDropped();
					WeaponInventory[Main2] = NewWeapon;
					NewWeapon->SetWeaponInventoryIndex(Main2);
				}

				// Weapon 장착여부 설정
				OwnerCharacter->SetCurrentWeapon(NewWeapon);

				// Main weapon socket에 부착
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
					주무기 슬롯(첫번째, 두번째) 중 두번째 슬롯에 무기가 없는 경우.
				*/

				// 2번째 슬롯에 Weapon 배정
				WeaponInventory[Main2] = NewWeapon;
				NewWeapon->SetWeaponInventoryIndex(Main2);

				// Secondary Weapon 소켓에 Weapon 부착
				NewWeapon->AttachSocketName = OwnerCharacter->GetThirdWeaponSocketName();
				OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);

			}


		}
		else
		{
			/*
				주무기 슬롯(첫번째, 두번째) 중 첫번째 슬롯에 주무기가 없는 경우.
			*/
			// 0번째 슬롯에 Weapon 배정
			WeaponInventory[Main1] = NewWeapon;
			NewWeapon->SetWeaponInventoryIndex(Main1);

			if (WeaponInventory[Main2])
			{
				/*
					주무기 슬롯(첫번째, 두번째) 중 두번째 슬롯에 무기가 있는 경우.
				*/
				if (OwnerCharacter->IsEquipWeapon())
				{
					// Secondary Weapon 소켓에 Weapon 부착
					NewWeapon->AttachSocketName = OwnerCharacter->GetSecondaryWeaponSocketName();
					OwnerCharacter->AttachWeapon(NewWeapon, NewWeapon->AttachSocketName);

				}
				else
				{
					OwnerCharacter->SetCurrentWeapon(NewWeapon);
					// Main Weapon 소켓에 Weapon 부착
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
					주무기 슬롯(첫번째, 두번째) 중 두번째 슬롯에도 무기가 없는 경우.
				*/
				OwnerCharacter->SetCurrentWeapon(NewWeapon);
				// Main Weapon 소켓에 Weapon 부착
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
				Grenade를 소지하지 않은 경우
			*/
			// Grenade 슬롯에 추가
			WeaponInventory[Grenade] = NewWeapon;
			NewWeapon->SetWeaponInventoryIndex(Grenade);
			// Grenade socket에 mesh 부착.
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
	// 올바른 Index값인지 확인
	if (!ItemList.IsValidIndex(InventoryIndex))
	{
		return;
	}

	// 해당 InventoryIndex에 Item포인터가 있는지 확인.
	if (nullptr == ItemList[InventoryIndex])
	{
		return;
	}

	ItemList[InventoryIndex]->SetItemOwner(nullptr);

	// Weapon인 경우 해당 WeaponInventory를 nullptr로 변경
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
			Remove인 경우
		*/
		// 해당 Item의 Remove 메소드 호출.
		ItemList[InventoryIndex]->OnRemoved();
	}

	// 해당 Index의 Item 포인터를 nullptr로 변경.
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

	// 기존 ItemList들의 Item포인터들을 백업.
	TArray<AZItem*> TempItemList(ItemList);
	//ItemList 재할당
	ItemList.Init(nullptr, NewMaxSize);
	// Item포인터들을 ItemList에 할당.
	for (int32 Index = 0; Index < NewMaxSize; ++Index)
	{
		ItemList[Index] = TempItemList[Index];
	}

	// 만약 ItemList에 들어가지 못한 Item들이 있다면 모두 Drop
	for (int32 Index = ItemList.Num(); Index < MaxSizeOfItemList; ++Index)
	{
		//Item Drop 코드 기재할 것.
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
			// 비어있는 공간의 Index 반환.
			return Index;
		}
	}

	// ItemList가 꽉찬 경우.
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

bool UZCharacterItemStatusComponent::ServerEquipWeapon_Validate(AZWeapon * NewWeapon)
{
	return true;
}

void UZCharacterItemStatusComponent::ServerEquipWeapon_Implementation(AZWeapon * NewWeapon)
{
	EquipWeapon(NewWeapon);
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

