// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


protected:
	UFUNCTION()
	void OnSliderValueChange(float NewValue);

	UFUNCTION()
	void OnResolutionSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnWindowModeSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

protected:
	UPROPERTY(BlueprintReadOnly)
	class UTextBlock* VolumeText;

	UPROPERTY(BlueprintReadOnly)
	class USlider* VolumeSlider;

	UPROPERTY(BlueprintReadOnly)
	class UComboBoxString* ResolutionList;

	UPROPERTY(BlueprintReadOnly)
	class UComboBoxString* WindowModeList;

protected:
	UPROPERTY(BlueprintReadWrite)
	int32 ResolutionX = 1920;
	
	UPROPERTY(BlueprintReadWrite)
	int32 ResolutionY = 1080;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EWindowMode::Type> WindowMode;
};
