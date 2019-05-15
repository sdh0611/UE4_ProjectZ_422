// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterItemStatusComponent.h"
#include "ZItem.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZInventoryWidget.h"


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
}


// Called when the game starts
void UZCharacterItemStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemList.Init(nullptr, MaxSizeOfItemList);

	OwnerCharacter = Cast<AZCharacter>(GetOwner());
}


// Called every frame
void UZCharacterItemStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UZCharacterItemStatusComponent::AddItem(AZItem * NewItem)
{
	auto Item = GetItemByName(NewItem->GetItemName());
	if (nullptr == Item || Item->IsItemQuantityMaximum())
	{
		// Item이 ItemList내에 없는 경우 or 해당Item이 최대 개수까지 보유하고 있을 때

		// ItemList 내의 빈 공간에 넣어줌.
		int32 AllocatedIndex = AllocateInventoryIndex();
		if (AllocatedIndex == -1)
		{
			// ItemList내에 빈 공간이 없는 경우.
			// 다시 Pickup Actor를 만들어 Drop시킴. -> 후일 Item Drop코드 기재할 것.

		}
		else
		{
			// ItemList내에 할당할 수 있는 공간이 있는 경우 
			// 해당 Index에 Item포인터 할당.
			ItemList[AllocatedIndex] = NewItem;
			NewItem->SetInventoryIndex(AllocatedIndex);
			NewItem->SetItemOwner(OwnerCharacter);

			// Inventory에 Update
			auto PlayerController = Cast<AZPlayerController>(NewItem->GetItemOwner()->GetController());
			if (PlayerController)
			{
				auto InventoryWidget = PlayerController->GetZHUD()->GetUserHUD()->GetInventoryWidget();
				if (InventoryWidget)
				{
					InventoryWidget->AddItemToInventory(NewItem);
				}

			}

		}

	}
	else
	{
		// 같은 Item이 ItemList내에 있는 경우
		
		// 해당 Item의 개수를 증가시켜줌.
		int32 Remain = Item->AdjustQuantity(NewItem->GetCurrentQuantityOfItem());

		if (Remain > 0)
		{
			// 해당 Item의 최대 개수를 초과했을 때
			// AddItem() 재귀호출
			AddItem(NewItem);
		}
		else
		{
			// 해당 Item의 최대 개수를 초과하지 않은 경우
			NewItem->Destroy();
		}

	}



}

void UZCharacterItemStatusComponent::RemoveItem(int32 InventoryIndex, bool bDestroy)
{
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

	// 파괴하도록 설정했다면 Destroy 메소드 실행.
	if (bDestroy)
	{
		ItemList[InventoryIndex]->Destroy();
	}

	// 해당 Index의 Item 포인터를 nullptr로 변경.
	ItemList[InventoryIndex] = nullptr;

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

