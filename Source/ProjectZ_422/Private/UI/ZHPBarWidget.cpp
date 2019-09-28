// Fill out your copyright notice in the Description page of Project Settings.


#include "ZHPBarWidget.h"
#include "ZPlayerStatusComponent.h"
#include "Components/ProgressBar.h"

void UZHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UZHPBarWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bCanUpdateBar)
	{
		InterpGage(InDeltaTime);
		UpdateWidget();
	}
	
}

void UZHPBarWidget::BindStatus(UZCharacterStatusComponent * NewStatusComponent)
{
	auto PlayerStatus = Cast<UZPlayerStatusComponent>(NewStatusComponent);
	if (nullptr == PlayerStatus)
	{
		ZLOG(Error, TEXT("Invalid status component.."));
		return;
	}

	StatusComponent = PlayerStatus;
	StatusComponent->OnStatusChanged.AddUObject(this, &UZHPBarWidget::SetUpdateBar);

	TargetHP = StatusComponent->GetCurrentHP();
	TargetDopingGage = StatusComponent->GetCurrentDopingGage();

	TargetHPRatio = TargetHP / StatusComponent->GetMaxHP();
	TargetDopingGageRatio = TargetDopingGage / StatusComponent->GetMaxDopingGage();

	UpdateWidget();
}

void UZHPBarWidget::InterpGage(float DeltaTime)
{
	if (TargetHP != StatusComponent->GetCurrentHP())
	{
		TargetHP = FMath::FInterpTo(TargetHP, StatusComponent->GetCurrentHP(), DeltaTime, 10.f);
		TargetHPRatio = TargetHP / StatusComponent->GetMaxHP();
	}

	if (TargetDopingGage != StatusComponent->GetCurrentDopingGage())
	{
		TargetDopingGage = FMath::FInterpTo(TargetDopingGage, StatusComponent->GetCurrentDopingGage(), DeltaTime, 10.f);
		TargetDopingGageRatio = TargetDopingGage / StatusComponent->GetMaxDopingGage();
	}

	if (TargetHP == StatusComponent->GetCurrentHP()
		&& TargetDopingGage == StatusComponent->GetCurrentDopingGage())
	{
		bCanUpdateBar = false;
	}

}

void UZHPBarWidget::SetUpdateBar()
{
	if (!StatusComponent.IsValid())
	{
		return;
	}

	bCanUpdateBar = true;
}
