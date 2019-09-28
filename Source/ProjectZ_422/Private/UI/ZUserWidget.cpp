// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserWidget.h"
#include "ZUserHUD.h"
#include "ZPlayerController.h"

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

	SetVisibility(ESlateVisibility::Collapsed);

}

FReply UZUserWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	auto Result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	//if (InKeyEvent.GetKey() == EKeys::Escape)
	//{
	//	ZLOG_S(Error);
	//	auto PC = GetOwningPlayer<AZPlayerController>();
	//	if (PC && PC->GetUserHUD())
	//	{
	//		PC->GetUserHUD()->RemoveWidgetFromList(this);
	//	}
	//	//OnRemoveScreen();
	//}

	return Result;
}
