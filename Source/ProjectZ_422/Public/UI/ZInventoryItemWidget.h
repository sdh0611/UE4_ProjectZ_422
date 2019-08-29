// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Input/Reply.h"
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

public:
	class AZItem* const GetBindingItem() const;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// Widget�� �ı��� �� ȣ���� �޼ҵ�. ��������Ʈ�� ȣ���� ����
	UFUNCTION(BlueprintCallable, Category = ItemWidget)
	void ClearWidget();
	
	// Item������ �����ϱ� ���� �޼ҵ�.
	void UpdateWidget();

	UFUNCTION()
	void OnDropButtonClicked();

protected:
	UPROPERTY(BlueprintReadWrite, Category = ItemWidget)
	class UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, Category = ItemWidget)
	class UTextBlock* ItemName;
	
	UPROPERTY(BlueprintReadWrite, Category = ItemWidget)
	class UTextBlock* QuantityOfItem;
	
	UPROPERTY(BlueprintReadWrite, Category = ItemWidget)
	class UButton* DropButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemWidget, Meta = (AllowPrivateAccess = true))
	class AZItem* Item;
};
