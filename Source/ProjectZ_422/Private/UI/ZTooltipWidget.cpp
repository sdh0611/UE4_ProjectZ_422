// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTooltipWidget.h"
#include "ZGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UZTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Name")));
	check(NewNameText);
	NameText = NewNameText;

	auto NewExplanationText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Explanation")));
	check(NewExplanationText);
	ExplanationText = NewExplanationText;

	auto NewItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Item")));
	check(NewItemImage);
	ItemImage = NewItemImage;

}

void UZTooltipWidget::BindItemInfo(const FString& NewItemName)
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(MyGameInstance);
	auto ItemData = MyGameInstance->GetItemDataByName(NewItemName);
	BindingItemData = ItemData;

	if (nullptr == ItemData)
	{
		//ClearWidget();
		ItemImage->SetBrushFromTexture(nullptr);
		NameText->SetText(FText::GetEmpty());
		ExplanationText->SetText(FText::GetEmpty());
	}
	else 
	{
		//UpdateWidget();
		ItemImage->SetBrushFromTexture(GetGameInstance<UZGameInstance>()->GetItemImage(NewItemName));
		NameText->SetText(FText::FromString(ItemData->ItemName));
		ExplanationText->SetText(FText::FromString(ItemData->ItemExplanation));
	}

}

void UZTooltipWidget::ClearWidget()
{
	NameText->SetText(FText::GetEmpty());
	ExplanationText->SetText(FText::GetEmpty());
}
