// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitlePlayerController.h"
#include "..\..\Public\Title\ZTitlePlayerController.h"
#include "Blueprint/UserWidget.h"

void AZTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	
	if (TitleWidgetClass)
	{
		TitleWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass);
		check(nullptr != TitleWidget);

		TitleWidget->AddToViewport();

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TitleWidget->GetCachedWidget());
		SetInputMode(InputMode);
	}

}
