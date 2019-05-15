// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterItemStatusComponent.h"
#include "ZItem.h"
#include "ZCharacter.h"


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
		// Item�� ItemList���� ���� ��� or �ش�Item�� �ִ� �������� �����ϰ� ���� ��

		// ItemList ���� �� ������ �־���.
		int32 AllocatedIndex = AllocateInventoryIndex();
		if (AllocatedIndex == -1)
		{
			// ItemList���� �� ������ ���� ���.
			// �ٽ� Pickup Actor�� ����� Drop��Ŵ. -> ���� Item Drop�ڵ� ������ ��.

		}
		else
		{
			// ItemList���� �Ҵ��� �� �ִ� ������ �ִ� ��� 
			// �ش� Index�� Item������ �Ҵ�.
			ItemList[AllocatedIndex] = NewItem;
			NewItem->SetInventoryIndex(AllocatedIndex);
			NewItem->SetItemOwner(Cast<AZCharacter>(GetOwner()));
		}

	}
	else
	{
		// ���� Item�� ItemList���� �ִ� ���
		
		// �ش� Item�� ������ ����������.
		int32 Remain = Item->AdjustQuantity(NewItem->GetCurrentQuantityOfItem());

		if (Remain > 0)
		{
			// �ش� Item�� �ִ� ������ �ʰ����� ��
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

void UZCharacterItemStatusComponent::RemoveItem(int32 InventoryIndex, bool bDestroy)
{
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

	// �ı��ϵ��� �����ߴٸ� Destroy �޼ҵ� ����.
	if (bDestroy)
	{
		ItemList[InventoryIndex]->Destroy();
	}

	// �ش� Index�� Item �����͸� nullptr�� ����.
	ItemList[InventoryIndex] = nullptr;

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

