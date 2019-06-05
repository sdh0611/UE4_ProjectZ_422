// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZCurrentWeaponInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZCurrentWeaponInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void BindWeapon(class AZWeapon* NewWeapon);
	void UpdateWidget();
	void ClearWidget();

private:
	UPROPERTY()
	class AZWeapon* Weapon;

	UPROPERTY()
	class UTextBlock* WeaponName;
	
	UPROPERTY()
	class UTextBlock* CurrentAmmo;

	UPROPERTY()
	class UTextBlock* MaxAmmo;


};
