// Fill out your copyright notice in the Description page of Project Settings.


#include "ZEndGameWidget.h"
#include "Components/TextBlock.h"

void UZEndGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EndText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_EndText")));
	check(EndText);

}

void UZEndGameWidget::SetEndText(const FString & NewText)
{
	if (EndText)
	{
		EndText->SetText(FText::FromString(NewText));
	}

}
