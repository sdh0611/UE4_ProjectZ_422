// Fill out your copyright notice in the Description page of Project Settings.


#include "ZHPBarWidget.h"
#include "ZCharacterStatusComponent.h"
#include "Components/ProgressBar.h"

void UZHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UZHPBarWidget::BindStatus(UZCharacterStatusComponent * NewStatusComponent)
{
	StatusComponent = NewStatusComponent;
	StatusComponent->OnStatusChanged.AddUObject(this, &UZHPBarWidget::UpdateWidget);
	UpdateWidget();
}
