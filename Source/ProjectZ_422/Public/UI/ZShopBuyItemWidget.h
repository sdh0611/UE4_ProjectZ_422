// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZNumberWidgetInterface.h"
#include "ZShopBuyItemWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FOnBuyShopItem, int32, int32);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZShopBuyItemWidget : public UUserWidget, public IZNumberWidgetInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	virtual void OnReceiveNumberInput(int32 NewNumber) override;

public:
	void SetParentShopWidget(class UZShopBuyWidget* NewParent);
	void BindShopItemData(struct FZShopItemData* NewShopItemData);

private:
	UFUNCTION()
	void OnBuyButtonClick();

public:
	FOnBuyShopItem OnBuyShopItem;

private:
	//struct FZShopItemData* ShopItemData;
	bool bIsDealOnlyOne = false;

	int32 ShopID = -1;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UTextBlock* ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UTextBlock* ItemPrice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	class UButton* BuyButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyItemWidget)
	TWeakObjectPtr<class UZShopBuyWidget> ParentShopWidget;

};
