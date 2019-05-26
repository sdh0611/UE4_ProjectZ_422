// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZShopBuyItemWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FOnBuyShopItem, struct FZShopItemData*, int32);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZShopBuyItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void BindShopItemData(struct FZShopItemData* NewShopItemData);

private:
	UFUNCTION()
	void OnBuyButtonClick();

public:
	FOnBuyShopItem OnBuyShopItem;

private:
	struct FZShopItemData* ShopItemData;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UTextBlock* ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UTextBlock* ItemPrice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UButton* BuyButton;

};
