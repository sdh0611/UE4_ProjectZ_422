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
	virtual void NativeConstruct() override;


public:
	UFUNCTION(BlueprintCallable)
	void BindItemInfo(const FString& NewItemName);

	UFUNCTION(BlueprintCallable)
	void ClearWidget();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidget();


protected:
	const FZItemData* BindingItemData;

protected:
	UPROPERTY()
	class UImage* ItemImage;
	
	UPROPERTY()
	class UTextBlock* NameText;

	UPROPERTY()
	class UTextBlock* ExplanationText;

};
