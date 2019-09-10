// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZWeaponInventoryItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZWeaponInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void BindWeapon(class AZWeapon* NewWeapon);

protected:
	UFUNCTION(BlueprintCallable)
	void ClearWidget();

	UFUNCTION(BlueprintCallable)
	void UpdateWidget();


protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class AZWeapon> BindingWeapon;

protected:
	UPROPERTY()
	class UImage* WeaponImage;

	UPROPERTY()
	class UTextBlock* SlotName;

};
