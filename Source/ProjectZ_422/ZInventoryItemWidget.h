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
	// ItemWidget�� Item�� ���ε��ϱ� ���� �޼ҵ�.
	void BindItem(class AZItem* NewItem);

private:
	// Widget�� �ı��� �� ȣ���� �޼ҵ�. ��������Ʈ�� ȣ���� ����
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
