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
#include "Json.h"
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

void AZShop::RequestShopData(int32 BuyItemShopID, int32 Quantity)
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (nullptr == MyGameInstance)
	{
		ZLOG(Error, TEXT("Invalid game instance.."));
		return;
	}

	FString PostParameter = FString::Printf(TEXT("shopid=%d&quantity=%d"), BuyItemShopID, Quantity);

	MyGameInstance->GetWebConnector().HttpPost(
		MyGameInstance->GetWebConnector().GetWebURL(),
		PostParameter,
		FHttpRequestCompleteDelegate::CreateUObject(this, &AZShop::OnShopResponseReceive)
	);

}

void AZShop::OnShopResponseReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	ZLOG_S(Error);
	if (bWasSuccessful)
	{
		ZLOG_S(Error);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool bResult = JsonObject->GetBoolField("result");
			if (!bResult)
			{
				ZLOG(Error, TEXT("Failed to get shop data.."));
				return;
			}
			
			FZShopItemData ShopItemData;
			ShopItemData.ShopID = JsonObject->GetIntegerField("shopid");
			ShopItemData.ItemName = JsonObject->GetStringField("itemname");
			ShopItemData.ItemPrice = JsonObject->GetIntegerField("itemprice");
			ShopItemData.ItemType = JsonObject->GetStringField("itemtype");
			ShopItemData.bIsDealOnlyOne = JsonObject->GetBoolField("isdealonlyone");

			int32 Quantity = JsonObject->GetIntegerField("quantity");

			BuyProcess(ShopItemData, Quantity);


		}
		else
		{
			ZLOG(Error, TEXT("Deserialize fail.."));
		}

	}

}

void AZShop::BuyProcess(FZShopItemData ShopItemData, int32 Quantity)
{



}

bool AZShop::ServerBuy_Validate(APlayerController * PC, int32 BuyItemShopID, int32 Quantity)
{
	return true;
}

void AZShop::ServerBuy_Implementation(APlayerController * PC, int32 BuyItemShopID, int32 Quantity)
{
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

	/* Shop data 가져오기 */
	auto BuyItemData = FindShopItemDataByID(BuyItemShopID);
	if (nullptr == BuyItemData)
	{
		ZLOG(Error, TEXT("ShopItemData not exist."));
		return;
	}


	/*
		돈계산
	*/
	int32 TotalQuantity = Quantity;
	int32 TotalPayment = (BuyItemData->ItemPrice) * Quantity;
	if (ItemStatusComponent->GetCurrentMoney() < TotalPayment)
	{
		ZLOG(Warning, TEXT("Money is not enough to buy item"));
		TotalQuantity = (ItemStatusComponent->GetCurrentMoney()) / (BuyItemData->ItemPrice);
		/* 1개도 못 살 정도면 return */
		if (TotalQuantity < 1)
		{
			return;
		}
		TotalPayment = (BuyItemData->ItemPrice) * TotalQuantity;
	}

	ItemStatusComponent->AdjustMoney(-TotalPayment);

	/*
		Item생성 및 ItemList에 추가
	*/
	TSubclassOf<AZItem> SpawnItemClass = nullptr;

	auto ZGameInstance = GetGameInstance<UZGameInstance>();
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
	while (RemainQuantity > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PC;
		AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(SpawnItemClass, SpawnParams);
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
		
		ZLOG_S(Error);
		if (RemainQuantity < ItemData->MaxQuantity)
		{
			NewItem->SetCurrentQuantityOfItem(RemainQuantity);
		}
		else
		{
			NewItem->SetCurrentQuantityOfItem(ItemData->MaxQuantity);
		}
		NewItem->InitItemData(ItemData);
		ItemStatusComponent->AddItem(NewItem);

		RemainQuantity -= ItemData->MaxQuantity;
	}

}

bool AZShop::ServerSell_Validate(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity)
{
	return true;
}

void AZShop::ServerSell_Implementation(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity)
{
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


	/*
		돈계산
		-> 판매 가격은 상점 구매 가격의 7할로 통일.
	*/
	int32 TotalQuantity = Quantity;
	int32 PurchaseMoney = ShopItemData->ItemPrice * 0.7f;
	if (SellItem->GetCurrentQuantityOfItem() < TotalQuantity)
	{
		/*
			입력된 판매 수량이 아이템 잔여 수량보다 많은 경우
			-> 잔여 수량만큼
		*/
		TotalQuantity = SellItem->GetCurrentQuantityOfItem();
	}
	ZLOG(Error, TEXT("Sell : %d"), TotalQuantity);
	PurchaseMoney *= TotalQuantity;

	ItemStatusComponent->AdjustMoney(PurchaseMoney);
	/*
		Item 수량 조정
	*/
	SellItem->AdjustQuantity(-TotalQuantity);

}
