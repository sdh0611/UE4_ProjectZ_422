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
	/*
		해당 Property는 이거 하나때문에 생성자 만들기 귀찮아서 그냥 에디터에서 할당해주기로함..
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


};
