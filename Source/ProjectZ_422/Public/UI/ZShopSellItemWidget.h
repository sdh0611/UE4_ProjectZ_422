// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZShopSellItemWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FOnSellItem, class AZItem*, int32);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZShopSellItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void BindItem(class AZItem* NewItem);
	void UpdateWidget();

private:
	void ClearWidget();

	UFUNCTION()
	void OnSellButtonClick();

public:
	FOnSellItem OnSellItem;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SellItemWidget, Meta = (AllowPrivateAccess = true))
	class AZItem* Item;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SellItemWidget)
	class UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SellItemWidget)
	class UTextBlock* ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SellItemWidget)
	class UTextBlock* ItemQuantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SellItemWidget)
	class UTextBlock* ItemPrice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SellItemWidget)
	class UButton* SellButton;
};