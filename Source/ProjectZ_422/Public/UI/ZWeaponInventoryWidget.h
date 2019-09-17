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
	void AddItemToWeaponInventory(const FName& Name, int32 WeaponInventoryIndex);
	void UpdateWidget();
	void ClearWidget(int32 WeaponInventoryIndex);

protected:
	class UTexture2D* const FindTexture(const FName& Name) const;

private:
	void LoadIconImage();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* IconDataTable;

	UPROPERTY()
	TMap<FName, UTexture2D*> IconTable;

protected:
	UPROPERTY()
	class UImage* WeaponMainFirst;
	
	UPROPERTY()
	class UImage* WeaponMainSecond;
	
	UPROPERTY()
	class UImage* WeaponGrenade;


};
