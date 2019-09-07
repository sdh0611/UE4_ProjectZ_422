// Fill out your copyright notice in the Description page of Project Settings.


#include "ZMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void AZMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(nullptr != MenuWidgetClass);

	MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
	check(nullptr != MenuWidget);

	MenuWidget->AddToViewport();

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MenuWidget->GetCachedWidget());

	SetInputMode(InputMode);

}

//void AZMenuPlayerController::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//
//}
