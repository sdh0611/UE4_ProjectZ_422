// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZUserHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZUserHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
public:
	// ȭ�鿡 Inventory�� ���� ���� �޼ҵ�.
	void DrawInventoryWidget();
	// ȭ�鿡�� Inventory�� ����� ���� �޼ҵ�.
	void RemoveInventoryWidget();

public:
	bool IsInventoryOnScreen() const;
	class UZInventoryWidget* const GetInventoryWidget() const;

private:
	UPROPERTY(BlueprintReadOnly, Category = UserHUD, Meta = (AllowPrivateAccess = true))
	bool bIsInventoryOnScreen;

	UPROPERTY()
	class UZInventoryWidget* InventoryWidget;


};
