// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZInventoryItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZInventoryItemWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;

public:
	// ItemWidget에 Item을 바인딩하기 위한 메소드.
	void BindItem(class AZItem* NewItem);

private:
	// Widget을 파괴할 때 호출할 메소드. 델리게이트로 호출할 예정
	UFUNCTION()
	void ClearWidget();

private:
	UPROPERTY()
	class UImage* ItemImage;

	UPROPERTY()
	class UTextBlock* ItemName;
	
	UPROPERTY()
	class UTextBlock* QuantityOfItem;

	UPROPERTY()
	class UButton* DropButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = InventoryItem, Meta = (AllowPrivateAccess = true))
	class AZItem* Item;
};
