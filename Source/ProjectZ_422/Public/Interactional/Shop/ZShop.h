// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/ZInteractional.h"
#include "ZShop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZShop : public AZInteractional
{
	GENERATED_BODY()
	
public:
	AZShop();

public:
	virtual void BeginPlay() override;
	virtual void OnInteraction(class AZCharacter* NewCharacter);
	virtual void OnFocus() override;
	virtual void OnFocusEnd() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void Buy(APawn* Pawn, int32 BuyItemShopID, int32 Quantity = 1);
	void Sell(APawn* Pawn, class AZItem* SellItem, int32 Quantity = 1);
	void OnExitShop();

	/* HalfTime에 상점 활성화 */
	void OpenShop();
	/* WaveTime 혹은 BossTime에 상점 비활성화 */
	void CloseShop();

	struct FZShopItemData* const FindShopItemDataByName(const FString& ShopItemName) const;
	struct FZShopItemData* const FindShopItemDataByID(int32 NewShopID) const;

private:
	void ConstructShopWidget(class AZCharacter* EnterCharacter);

protected:
	/* Client to server RPC */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBuy(APawn* Pawn, int32 BuyItemShopID, int32 Quantity);
	bool ServerBuy_Validate(APawn* Pawn, int32 BuyItemShopID, int32 Quantity);
	void ServerBuy_Implementation(APawn* Pawn, int32 BuyItemShopID, int32 Quantity);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSell(APawn* Pawn, class AZItem* SellItem, int32 Quantity = 1);
	bool ServerSell_Validate(APawn* Pawn, class AZItem* SellItem, int32 Quantity = 1);
	void ServerSell_Implementation(APawn* Pawn, class AZItem* SellItem, int32 Quantity = 1);

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
	UPROPERTY(VisibleAnywhere, Category = Shop)
	class UBoxComponent* BodyCollision;

	UPROPERTY(VisibleAnywhere, Category = Shop)
	class USkeletalMeshComponent* BodyMesh;


};
