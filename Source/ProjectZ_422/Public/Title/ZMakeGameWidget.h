// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZMakeGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZMakeGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnCreateGameButtonClick();

protected:
	UPROPERTY()
	class UEditableTextBox* SessionName;

	UPROPERTY()
	class UEditableTextBox* ConnectNumber;

	UPROPERTY()
	class UButton* OK;

};
