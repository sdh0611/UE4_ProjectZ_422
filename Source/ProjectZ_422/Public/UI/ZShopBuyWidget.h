// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZShopBuyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZShopBuyWidget : public UUserWidget
{
	GENERATED_BODY()
	


public:
	virtual void NativeConstruct() override;

public:
	class UZShopBuyItemWidget* AddItem(struct FZShopItemData* NewItemData);
	void ClearWidget();

public:
	class UZTooltipWidget* const GetToolTipWidget() const;

public:
	/*
		�ش� Property�� �̰� �ϳ������� ������ ����� �����Ƽ� �׳� �����Ϳ��� �Ҵ����ֱ����..
	*/
	UPROPERTY(EditAnywhere, Category = BuyWidget)
	TSubclassOf<class UZShopBuyItemWidget> BuyItemWidgetClass;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* WeaponHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* RecoveryHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* DopingHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* AmmoHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UZTooltipWidget* ShopToolTipWidget;

};
