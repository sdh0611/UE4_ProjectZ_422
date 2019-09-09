// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTooltipWidget.h"
#include "ZGameInstance.h"

void UZTooltipWidget::BindItemInfo(const FString& NewItemName)
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(MyGameInstance);
	auto ItemData = MyGameInstance->GetItemDataByName(NewItemName);
	BindingItemData = ItemData;

	if (nullptr == ItemData)
	{
		ClearWidget();
	}
	else 
	{
		UpdateWidget();
	}

}
