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

public:
	class AZItem* const GetBindingItem() const;

private:
	// DropButton 바인딩 메소드. 
	/*
		NOTE(5.18) : 폐기예정 -> Blueprint로 구현을 옮김.
	*/
	UFUNCTION()
	void OnDropButtonClicked();

	// Widget을 파괴할 때 호출할 메소드. 델리게이트로 호출할 예정
	UFUNCTION(BlueprintCallable, Category = ItemWidget)
	void ClearWidget();
	
	// Item정보를 갱신하기 위한 메소드.
	void UpdateWidget();

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
