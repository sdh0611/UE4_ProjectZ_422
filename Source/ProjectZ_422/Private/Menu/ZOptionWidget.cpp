// Fill out your copyright notice in the Description page of Project Settings.


#include "ZOptionWidget.h"
#include "ZGameInstance.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"

void UZOptionWidget::NativeConstruct()
{
	VolumeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Volume")));
	check(VolumeText);

	VolumeSlider = Cast<USlider>(GetWidgetFromName(TEXT("VolumeSlider")));
	check(VolumeSlider);

	ResolutionList = Cast<UComboBoxString>(GetWidgetFromName(TEXT("ResolutionList")));
	check(ResolutionList);

	WindowModeList = Cast<UComboBoxString>(GetWidgetFromName(TEXT("WindowModeList")));
	check(WindowModeList);

	ResolutionList->AddOption(TEXT("1920x1080"));
	ResolutionList->AddOption(TEXT("1600x900"));
	ResolutionList->AddOption(TEXT("1280x720"));
	ResolutionList->SetSelectedIndex(0);

	WindowModeList->AddOption(TEXT("전체화면"));
	WindowModeList->AddOption(TEXT("창모드"));
	WindowModeList->SetSelectedIndex(0);

	WindowMode = EWindowMode::Fullscreen;

	ResolutionList->OnSelectionChanged.AddDynamic(this, &UZOptionWidget::OnResolutionSelected);
	WindowModeList->OnSelectionChanged.AddDynamic(this, &UZOptionWidget::OnWindowModeSelected);
}

void UZOptionWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UZOptionWidget::OnSliderValueChange(float NewValue)
{
	int32 Value = static_cast<int32>(NewValue*100.f);
	VolumeText->SetText(FText::FromString(FString::FromInt(Value)));

}

void UZOptionWidget::OnResolutionSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (0 == SelectedItem.Compare(TEXT("1920x1080")))
	{
		ZLOG_S(Error);
		ResolutionX = 1920;
		ResolutionY = 1080;
	}
	else if (0 == SelectedItem.Compare(TEXT("1600x900")))
	{
		ResolutionX = 1600;
		ResolutionY = 900;
	}
	else if (0 == SelectedItem.Compare(TEXT("1280x720")))
	{
		ZLOG_S(Error);
		ResolutionX = 1280;
		ResolutionY = 720;
	}

	//GEngine->GetGameUserSettings()->RequestResolutionChange(ResolutionX, ResolutionY, WindowMode);
	GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(ResolutionX, ResolutionY));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void UZOptionWidget::OnWindowModeSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (0 == SelectedItem.Compare(TEXT("전체화면")))
	{
		ZLOG_S(Error);
		WindowMode = EWindowMode::Fullscreen;
	}
	else if (0 == SelectedItem.Compare(TEXT("창모드")))
	{
		ZLOG_S(Error);
		WindowMode = EWindowMode::Windowed;
	}

	//GEngine->GetGameUserSettings()->RequestResolutionChange(ResolutionX, ResolutionY, WindowMode);
	GEngine->GetGameUserSettings()->SetFullscreenMode(WindowMode);
	GEngine->GetGameUserSettings()->ApplySettings(true);

}
