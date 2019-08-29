// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZShopSellWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZShopSellWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	class UZShopSellItemWidget* AddItem(class AZItem* NewItem);
	void ClearWidget();

public:
	/*
		�ش� Property�� �̰� �ϳ������� ������ ����� �����Ƽ� �׳� �����Ϳ��� �Ҵ����ֱ����..
	*/
	UPROPERTY(EditAnywhere, Category = BuyWidget)
	TSubclassOf<class UZShopSellItemWidget> SellItemWidgetClass;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* WeaponHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* RecoveryHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* DopingHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* AmmoHolder;


};
