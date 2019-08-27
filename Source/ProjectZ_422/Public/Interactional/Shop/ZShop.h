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

public:
	void Buy(struct FZShopItemData* BuyItemData, int32 Quantity = 1);
	void Sell(class AZItem* SellItem, int32 Quantity = 1);
	void OnExitShop();

	/* HalfTime에 상점 활성화 */
	void OpenShop();
	/* WaveTime 혹은 BossTime에 상점 비활성화 */
	void CloseShop();

	struct FZShopItemData* const FindShopItemData(const FString& ShopItemName) const;

public:
	/* Setter */
	void SetEnterPlayer(class AZCharacter* NewPlayer);

	/* Getter */
	class AZCharacter* const GetEnterPlayer() const;

private:
	void ConstructShopWidget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shop)
	bool bIsShopOpen;

protected:
	/* Weapon */
	UPROPERTY(EditAnywhere, Category = Shop)
	TSubclassOf<class AZItem> GunClass;

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

private:
	UPROPERTY(VisibleAnywhere, Category = Shop)
	class AZCharacter* EnterPlayer;

	UPROPERTY()
	class UDataTable* ShopItemDataTable;


protected:
	UPROPERTY(VisibleAnywhere, Category = Shop)
	class UBoxComponent* BodyCollision;

	UPROPERTY(VisibleAnywhere, Category = Shop)
	class USkeletalMeshComponent* BodyMesh;


};
