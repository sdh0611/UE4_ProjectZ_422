// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void BindItemInfo(const FString& NewItemName);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ClearWidget();

protected:
	const FZItemData* BindingItemData;

};
