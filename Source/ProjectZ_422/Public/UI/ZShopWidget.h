// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZUserWidget.h"
#include "ZShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZShopWidget : public UZUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnDrawScreen() override;
	virtual void OnRemoveScreen() override;
	
public:
	void BindShop(class AZShop* NewShop);
	void ConstructBuyWidget(class UDataTable* const ShopItemData);
	void ConstructSellWidget(const TArray<class AZItem*>& ItemList);
	void AddItemToBuyWidget(struct FZShopItemData* const NewShopItemData);
	void AddItemToSellWidget(class AZItem* NewItem);
	void UpdateCurrentMoney(int32 NewMoney);

private:


private:
	UFUNCTION()
	void OnExitButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ShopWidget)
	class UButton* ExitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ShopWidget)
	class UTextBlock* CurrentMoney;

	UPROPERTY(VisibleAnywhere, Category = Shop)
	class UZShopBuyWidget* ShopBuyWidget;

	UPROPERTY(VisibleAnywhere, Category = Shop)
	class UZShopSellWidget* ShopSellWidget;

private:
	//UPROPERTY(VisibleAnywhere, Category = ShopWidget)
	//class AZShop* Shop;

};
