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
	void AddItem(class AZItem* NewItem);
	void ClearWidget();

public:
	/*
		해당 Property는 이거 하나때문에 생성자 만들기 귀찮아서 그냥 에디터에서 할당해주기로함..
	*/
	UPROPERTY(EditAnywhere, Category = BuyWidget)
	TSubclassOf<class UZShopSellItemWidget> SellItemWidgetClass;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 HolderSize = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* WeaponHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* RecoveryHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* DopingHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuyWidget)
	class UScrollBox* AmmoHolder;


};
