// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitlePlayerController.h"
#include "..\..\Public\Title\ZTitlePlayerController.h"
#include "Blueprint/UserWidget.h"

void AZTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		bShowMouseCursor = true;

		if (TitleWidgetClass)
		{
			TitleWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass);

			if (TitleWidget)
			{
				TitleWidget->AddToViewport();

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(TitleWidget->GetCachedWidget());
				SetInputMode(InputMode);
			}
		}
	}

}
