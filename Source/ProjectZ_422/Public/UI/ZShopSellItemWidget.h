// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZNumberWidgetInterface.h"
#include "ZShopSellItemWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FOnSellItem, int32, int32);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZShopSellItemWidget : public UUserWidget, public IZNumberWidgetInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnReceiveNumberInput(int32 NewNumber) override;

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
	UPROPERTY(BlueprintReadOnly, Category = SellItemWidget, Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<class AZItem> Item;

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
