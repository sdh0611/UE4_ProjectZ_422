// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/ZInteractional.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "ZShop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZShop : public AActor
{
	GENERATED_BODY()
	
public:
	AZShop();

public:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void Buy(APlayerController* PC, int32 BuyItemShopID, int32 Quantity = 1);
	void Sell(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity = 1);
	void OpenShop(class AZPlayerController* NewCharacter);

	struct FZShopItemData* const FindShopItemDataByName(const FString& ShopItemName) const;
	struct FZShopItemData* const FindShopItemDataByID(int32 NewShopID) const;

private:
	void RequestShopData(int32 BuyItemShopID, int32 Quantity);
	void OnShopResponseReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void BuyProcess(FZShopItemData ShopItemData, int32 Quantity);

protected:
	/* Client to server RPC */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBuy(APlayerController* PC, int32 BuyItemShopID, int32 Quantity);
	bool ServerBuy_Validate(APlayerController* PC, int32 BuyItemShopID, int32 Quantity);
	void ServerBuy_Implementation(APlayerController* PC, int32 BuyItemShopID, int32 Quantity);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSell(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity);
	bool ServerSell_Validate(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity);
	void ServerSell_Implementation(APlayerController* PC, int32 SellItemInventoryIndex, int32 Quantity);

	/* Server to client RPC */


	/* Replicated using */

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shop, Replicated)
	bool bIsShopOpen;

protected:
	/* Weapon */
	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> ARClass;

	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> ShotgunClass;

	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> KnifeClass;

	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> GrenadeClass;

	/* Recovery */
	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> RecoveryClass;

	/* Doping */
	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> DopingClass;

	/* Doping */
	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> AmmoClass;

private:
	UPROPERTY()
	class UDataTable* ShopItemDataTable;


protected:
	//UPROPERTY(VisibleAnywhere, Category = Shop)
	//class UBoxComponent* BodyCollision;

	//UPROPERTY(VisibleAnywhere, Category = Shop)
	//class USkeletalMeshComponent* BodyMesh;


};
