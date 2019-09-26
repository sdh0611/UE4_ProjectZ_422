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
	void Buy(APawn* Pawn, struct FZShopItemData* BuyItemData, int32 Quantity = 1);
	void Sell(APawn* Pawn, class AZItem* SellItem, int32 Quantity = 1);
	void OnExitShop();

	/* HalfTime�� ���� Ȱ��ȭ */
	void OpenShop();
	/* WaveTime Ȥ�� BossTime�� ���� ��Ȱ��ȭ */
	void CloseShop();

	struct FZShopItemData* const FindShopItemData(const FString& ShopItemName) const;

private:
	void ConstructShopWidget(class AZCharacter* EnterCharacter);

protected:
	/* Client to server RPC */

	/* Server to client RPC */
	UFUNCTION(Client, Reliable)
	void ClientConstructShopWidget(class AZCharacter* EnterCharacter);
	void ClientConstructShopWidget_Implementation(class AZCharacter* EnterCharacter);


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
