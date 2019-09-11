// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserWidget.h"

void UZUserWidget::OnDrawScreen()
{
	SetVisibility(ESlateVisibility::Visible);

}

void UZUserWidget::OnRemoveScreen()
{
	if (bCanDestruct)
	{
		RemoveFromParent();
		return;
	}

	SetVisibility(ESlateVisibility::Hidden);

}
