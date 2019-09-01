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
#include "ConstructorHelpers.h"


AZShop::AZShop()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsShopOpen = false;

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyCollision"));
	RootComponent = BodyCollision;

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh>
	//	SM_SHOP(TEXT("StaticMesh'/Engine/EditorMeshes/ColorCalibrator/SM_ColorCalibrator.SM_ColorCalibrator'"));
	//if(SM_SHOP.Succeeded())
	//{
	//	Mesh->SetStaticMesh(SM_SHOP.Object);
	//}

	//WeaponClass = AZWeapon::StaticClass();
	//RecoveryClass = AZRecovery::StaticClass();
	//DopingClass = AZDoping::StaticClass();

}

void AZShop::BeginPlay()
{
	Super::BeginPlay();

	auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(nullptr != ZGameInstance);
	ShopItemDataTable = ZGameInstance->GetShopItemDataTable();

}

void AZShop::OnInteraction(AZCharacter * NewCharacter)
{
	if (!bIsShopOpen)
	{
		return;
	}

	if (NewCharacter)
	{
		if (EnterPlayer == NewCharacter)
		{
			return;
		}

		auto PlayerController = Cast<AZPlayerController>(NewCharacter->GetController());
		if (nullptr == PlayerController)
		{
			return;
		}
		PlayerController->GetZHUD()->GetUserHUD()->GetShopWidget()->BindShop(this);
		PlayerController->GetZHUD()->GetUserHUD()->DrawShopWidget();
	}

	SetEnterPlayer(NewCharacter);

	ConstructShopWidget();

	Super::OnInteraction(NewCharacter);
}

void AZShop::Buy(FZShopItemData* BuyItemData, int32 Quantity)
{
	auto ItemStatusComponent = EnterPlayer->GetItemStatusComponent();
	if (nullptr == ItemStatusComponent)
	{
		ZLOG(Error, TEXT("ItemStatusComponent not exist."));
		return;
	}

	/*
		�����
		-> ���� ������ ��쿣 �״�� �޼ҵ� return
	*/
	if (ItemStatusComponent->GetCurrentMoney() < BuyItemData->ItemPrice)
	{
		ZLOG(Warning, TEXT("Money is not enough to buy item"));
		return;
	}
	ItemStatusComponent->AdjustMoney(-BuyItemData->ItemPrice);

	/*
		Item���� �� ItemList�� �߰�
	*/
	TSubclassOf<AZItem> SpawnItemClass = nullptr;

	auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(nullptr != ZGameInstance);

	const FZItemData* ItemData = nullptr;;

	switch (GetItemTypeFromString(BuyItemData->ItemType))
	{
		case EItemType::Weapon:
		{
			auto WeaponData = ZGameInstance->GetWeaponDataByName(BuyItemData->ItemName);
			switch (GetWeaponCategoryFromString(WeaponData->WeaponCategory))
			{
				case EWeaponCategory::Gun:
				{
					ZLOG(Error, TEXT("Buy Gun."));
					ItemData = ZGameInstance->GetGunDataByName(BuyItemData->ItemName);
					SpawnItemClass = GunClass;
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
					ItemData = ZGameInstance->GetGrenadeDataByName(BuyItemData->ItemName);
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
			ItemData = ZGameInstance->GetRecoveryDataByName(BuyItemData->ItemName);
			SpawnItemClass = RecoveryClass;
			break;
		}
		case EItemType::Doping:
		{
			ItemData = ZGameInstance->GetDopingDataByName(BuyItemData->ItemName);
			SpawnItemClass = DopingClass;
			break;
		}
		case EItemType::Ammo:
		{
			ItemData = ZGameInstance->GetAmmoDataByName(BuyItemData->ItemName);
			SpawnItemClass = AmmoClass;
			break;
		}
		default:
		{
			ZLOG(Error, TEXT("Invalid ItemType"));
			return;
		}
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
	NewItem->SetCurrentQuantityOfItem(Quantity);
	ItemStatusComponent->AddItem(NewItem);

}

void AZShop::Sell(AZItem* SellItem, int32 Quantity)
{
	if (Quantity < 1)
	{
		ZLOG(Error, TEXT("Invalid quantity value."));
		return;
	}

	auto ItemStatusComponent = EnterPlayer->GetItemStatusComponent();
	if (nullptr == ItemStatusComponent)
	{
		ZLOG(Error, TEXT("ItemStatusComponent not exist."));
		return;
	}

	auto ShopItemData = FindShopItemData(SellItem->GetItemName());
	if (nullptr == ShopItemData)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	/*
		�����
		-> �Ǹ� ������ ���� ���� ������ 7�ҷ� ����.
	*/
	int32 PurchaseMoney = ShopItemData->ItemPrice * 0.7f;
	if (SellItem->GetCurrentQuantityOfItem() < Quantity)
	{
		/*
			�Էµ� �Ǹ� ������ ������ �ܿ� �������� ���� ���
			-> �ܿ� ������ŭ �� ����
		*/
		PurchaseMoney *= SellItem->GetCurrentQuantityOfItem();
	}
	else
	{
		/*
			�Էµ� �Ǹż����� ������ �ܿ� �������� ���ų� ���� ���
			-> �Էµ� �Ǹż��� ��ŭ �� ����.
		*/
		PurchaseMoney *= Quantity;
	}

	ItemStatusComponent->AdjustMoney(PurchaseMoney);
	/*
		Item ���� ����
	*/
	SellItem->AdjustQuantity(-Quantity);
	
}

void AZShop::OnExitShop()
{
	if (EnterPlayer)
	{
		auto PlayerController = Cast<AZPlayerController>(EnterPlayer->GetController());
		if (nullptr == PlayerController)
		{
			return;
		}

		PlayerController->GetZHUD()->GetUserHUD()->RemoveShopWidget();
	}

	SetEnterPlayer(nullptr);
}

void AZShop::OpenShop()
{
	bIsShopOpen = true;
}

void AZShop::CloseShop()
{
	bIsShopOpen = false;
}

FZShopItemData * const AZShop::FindShopItemData(const FString & ShopItemName) const
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

void AZShop::SetEnterPlayer(AZCharacter * NewPlayer)
{
	EnterPlayer = NewPlayer;
}

AZCharacter * const AZShop::GetEnterPlayer() const
{
	return EnterPlayer;
}


void AZShop::ConstructShopWidget()
{
	ZLOG_S(Warning);
	
	auto PlayerController = Cast<AZPlayerController>(EnterPlayer->GetController());
	if (nullptr == PlayerController)
	{
		return;
	}

	auto ShopWidget = Cast<UZShopWidget>(PlayerController->GetZHUD()->GetUserHUD()->GetShopWidget());
	if (nullptr == ShopWidget)
	{
		return;
	}

	auto ItemStatusComponent = EnterPlayer->GetItemStatusComponent();

	ShopWidget->ConstructBuyWidget(ShopItemDataTable);
	ShopWidget->ConstructSellWidget(ItemStatusComponent->GetItemList());

}
