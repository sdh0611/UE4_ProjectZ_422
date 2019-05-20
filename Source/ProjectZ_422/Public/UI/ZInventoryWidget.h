// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZInventoryWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;

public:
	void AddItemToInventory(class AZItem* const NewItem);

private:
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	TSubclassOf<class UZInventoryItemWidget> InventoryItemWidgetClass;

private:
	UPROPERTY()
	class UScrollBox* ItemHolder;

};
