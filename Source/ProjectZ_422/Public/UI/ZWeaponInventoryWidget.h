// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZWeaponInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZWeaponInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
public:
	void AddItemToWeaponInventory(class AZWeapon* NewWeapon);
	void UpdateWidget();

protected:
	UPROPERTY()
	class UZWeaponInventoryItemWidget* WeaponMainFirst;
	
	UPROPERTY()
	class UZWeaponInventoryItemWidget* WeaponMainSecond;
	
	UPROPERTY()
	class UZWeaponInventoryItemWidget* WeaponGrenade;


};
