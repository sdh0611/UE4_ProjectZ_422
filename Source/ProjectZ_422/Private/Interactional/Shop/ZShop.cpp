// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShop.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZShopWidget.h"
#include "ZGameInstance.h"
#include "ZWeapon.h"
#include "ZGun.h"
#include "ZGrenade.h"
#include "ZRecovery.h"
#include "ZDoping.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AZShop::AZShop()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsShopOpen = true;

}

void AZShop::BeginPlay()
{
	Super::BeginPlay();

	auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(nullptr != ZGameInstance);
	ShopItemDataTable = ZGameInstance->GetShopItemDataTable();

	//if (!HasAuthority())
	//{
	//	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//	SetOwner(PC);
	//}

}

void AZShop::OpenShop(AZPlayerController* NewPC)
{
	ZLOG_S(Error);
	if (!bIsShopOpen)
	{
		return;
	}

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ShopInteraction."));
	if (NewPC)
	{
		//ConstructShopWidget(Cast<AZCharacter>(NewPC->GetPawn()));
	}

}

void AZShop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZShop, bIsShopOpen);


}

void AZShop::Buy(APlayerController* PC, int32 BuyItemShopID, int32 Quantity)
{
	/* Server RPC */
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT(" Buy"));
	ServerBuy(PC, BuyItemShopID, Quantity);
}

void AZShop::Sell(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity)
{
	/* Server RPC */
	ServerSell(PC, SellItemInventoryIndex, Quantity);
}


FZShopItemData * const AZShop::FindShopItemDataByName(const FString & ShopItemName) const
{
	TArray<FName> Names = ShopItemDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto ShopItemData = ShopItemDataTable->FindRow<FZShopItemData>(Name, TEXT(""));
		if (ShopItemData->ItemName == ShopItemName)
		{
			return ShopItemData;
		}
	}

	return nullptr;
}

FZShopItemData * const AZShop::FindShopItemDataByID(int32 NewShopID) const
{
	TArray<FName> Names = ShopItemDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto ShopItemData = ShopItemDataTable->FindRow<FZShopItemData>(Name, TEXT(""));
		if (ShopItemData->ShopID == NewShopID)
		{
			return ShopItemData;
		}
	}

	return nullptr;
}

//void AZShop::ConstructShopWidget(AZCharacter* EnterCharacter)
//{
//	ZLOG_S(Warning);
//	if (nullptr == EnterCharacter)
//	{
//		return;
//	}
//
//	/* ���� ��ü Ʋ �׸�. */
//	auto PlayerController = Cast<AZPlayerController>(EnterCharacter->GetController());
//	if (nullptr == PlayerController)
//	{
//		return;
//	}
//	PlayerController->GetUserHUD()->GetShopWidget()->BindShop(this);
//	PlayerController->GetUserHUD()->DrawShopWidget();
//	
//	/* ���� �����۵��� �׸��� ���� �غ�. */
//	auto ShopWidget = Cast<UZShopWidget>(PlayerController->GetUserHUD()->GetShopWidget());
//	if (nullptr == ShopWidget)
//	{
//		return;
//	}
//
//	auto ItemStatusComponent = EnterCharacter->GetItemStatusComponent();
//	if (nullptr == ItemStatusComponent)
//	{
//		ZLOG(Error, TEXT("ItemStatusComponent not exist.."));
//		return;
//	}
//
//	ShopWidget->ConstructBuyWidget(ShopItemDataTable);
//	ShopWidget->ConstructSellWidget(ItemStatusComponent->GetItemList());
//
//}

bool AZShop::ServerBuy_Validate(APlayerController * PC, int32 BuyItemShopID, int32 Quantity)
{
	return true;
}

void AZShop::ServerBuy_Implementation(APlayerController * PC, int32 BuyItemShopID, int32 Quantity)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Buy Item process."));
	auto Player = Cast<AZCharacter>(PC->GetPawn());
	if (nullptr == Player)
	{
		ZLOG(Error, TEXT("Invalid pawn."));
		return;
	}

	if (Quantity < 1)
	{
		ZLOG(Error, TEXT("Invalid quantity value."));
		return;
	}

	auto ItemStatusComponent = Player->GetItemStatusComponent();
	if (nullptr == ItemStatusComponent)
	{
		ZLOG(Error, TEXT("ItemStatusComponent not exist."));
		return;
	}

	/* Shop data �������� */
	auto BuyItemData = FindShopItemDataByID(BuyItemShopID);
	if (nullptr == BuyItemData)
	{
		ZLOG(Error, TEXT("ShopItemData not exist."));
		return;
	}

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("In Buy Item process."));

	/*
		�����
	*/
	int32 TotalQuantity = Quantity;
	int32 TotalPayment = (BuyItemData->ItemPrice) * Quantity;
	if (ItemStatusComponent->GetCurrentMoney() < TotalPayment)
	{
		ZLOG(Warning, TEXT("Money is not enough to buy item"));
		TotalQuantity = (ItemStatusComponent->GetCurrentMoney()) / (BuyItemData->ItemPrice);
		/* 1���� �� �� ������ return */
		if (TotalQuantity < 1)
		{
			return;
		}
		TotalPayment = (BuyItemData->ItemPrice) * TotalQuantity;
	}

	ItemStatusComponent->AdjustMoney(-TotalPayment);

	/*
		Item���� �� ItemList�� �߰�
	*/
	TSubclassOf<AZItem> SpawnItemClass = nullptr;

	auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(nullptr != ZGameInstance);

	const FZItemData* ItemData = ZGameInstance->GetItemDataByName(BuyItemData->ItemName);

	switch (GetItemTypeFromString(BuyItemData->ItemType))
	{
	case EItemType::Weapon:
	{
		auto WeaponData = ZGameInstance->GetWeaponDataByName(BuyItemData->ItemName);
		check(nullptr != WeaponData);

		switch (GetWeaponCategoryFromString(WeaponData->WeaponCategory))
		{
		case EWeaponCategory::Gun:
		{
			auto GunData = ZGameInstance->GetGunDataByName(BuyItemData->ItemName);
			check(GunData != nullptr);

			switch (GetGunTypeFromString(GunData->GunType))
			{
			case EGunType::AR:
			{
				SpawnItemClass = ARClass;
				break;
			}
			case EGunType::Shotgun:
			{
				SpawnItemClass = ShotgunClass;
				break;
			}
			}

			break;
		}
		case EWeaponCategory::Knife:
		{
			//SpawnItemClass = AZKnife::StaticClass();
			break;
		}
		case EWeaponCategory::Grenade:
		{
			ZLOG(Error, TEXT("Buy Grenade."));
			SpawnItemClass = GrenadeClass;
			break;
		}
		default:
		{
			ZLOG(Error, TEXT("Invalid type."));
			return;
		}
		}

		break;
	}
	case EItemType::Recovery:
	{
		SpawnItemClass = RecoveryClass;
		break;
	}
	case EItemType::Doping:
	{
		SpawnItemClass = DopingClass;
		break;
	}
	case EItemType::Ammo:
	{
		SpawnItemClass = AmmoClass;
		break;
	}
	default:
	{
		ZLOG(Error, TEXT("Invalid ItemType"));
		return;
	}
	}

	int32 RemainQuantity = TotalQuantity;
	while (RemainQuantity > ItemData->MaxQuantity)
	{
		AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(SpawnItemClass);
		if (nullptr == NewItem)
		{
			ZLOG(Error, TEXT("Failed to spawn item."));
			return;
		}

		if (nullptr == ItemData)
		{
			ZLOG(Error, TEXT("Invalid item data."));
			return;
		}
		ZLOG(Error, TEXT("Name : %s"), *ItemData->ItemName);

		NewItem->InitItemData(ItemData);
		NewItem->SetCurrentQuantityOfItem(ItemData->MaxQuantity);
		ItemStatusComponent->AddItem(NewItem);
		RemainQuantity -= ItemData->MaxQuantity;
	}

	AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(SpawnItemClass);
	if (nullptr == NewItem)
	{
		ZLOG(Error, TEXT("Failed to spawn item."));
		return;
	}

	if (nullptr == ItemData)
	{
		ZLOG(Error, TEXT("Invalid item data."));
		return;
	}
	ZLOG(Error, TEXT("Name : %s"), *ItemData->ItemName);

	NewItem->InitItemData(ItemData);
	NewItem->SetCurrentQuantityOfItem(RemainQuantity);
	ItemStatusComponent->AddItem(NewItem);

}

bool AZShop::ServerSell_Validate(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity)
{
	return true;
}

void AZShop::ServerSell_Implementation(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Sell Item process."));
	if (nullptr == PC)
	{
		return;
	}

	auto Player = Cast<AZCharacter>(PC->GetPawn());
	if (nullptr == Player)
	{
		ZLOG(Error, TEXT("Invalid pawn."));
		return;
	}

	if (Quantity < 1)
	{
		ZLOG(Error, TEXT("Invalid quantity value."));
		return;
	}

	int32 TotalQuantity = Quantity;

	auto ItemStatusComponent = Player->GetItemStatusComponent();
	if (nullptr == ItemStatusComponent)
	{
		ZLOG(Error, TEXT("ItemStatusComponent not exist."));
		return;
	}

	AZItem* SellItem = ItemStatusComponent->GetItemByIndex(SellItemInventoryIndex);
	if (nullptr == SellItem)
	{
		return;
	}

	auto ShopItemData = FindShopItemDataByName(SellItem->GetItemName());
	if (nullptr == ShopItemData)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("In Sell Item process."));
	/*
		�����
		-> �Ǹ� ������ ���� ���� ������ 7�ҷ� ����.
	*/
	int32 PurchaseMoney = ShopItemData->ItemPrice * 0.7f;
	if (SellItem->GetCurrentQuantityOfItem() < TotalQuantity)
	{
		/*
			�Էµ� �Ǹ� ������ ������ �ܿ� �������� ���� ���
			-> �ܿ� ������ŭ
		*/
		TotalQuantity = SellItem->GetCurrentQuantityOfItem();
	}

	PurchaseMoney *= TotalQuantity;

	ItemStatusComponent->AdjustMoney(PurchaseMoney);
	/*
		Item ���� ����
	*/
	SellItem->AdjustQuantity(-TotalQuantity);

}
