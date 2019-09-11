// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZUserWidget.h"
#include "ZInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZInventoryWidget : public UZUserWidget
{
	GENERATED_BODY()
	
public:
	UZInventoryWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;
	

public:
	void AddItemToInventory(class AZItem* const NewItem);

	void AddItemToWeaponInventory(class AZWeapon* const NewWeapon);


private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<class UZInventoryItemWidget> InventoryItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<class UZWeaponInventoryItemWidget> WeaponInventoryItemWidgetClass;

private:
	UPROPERTY()
	class UScrollBox* ItemHolder;

	UPROPERTY()
	class UScrollBox* WeaponHolder;

};
