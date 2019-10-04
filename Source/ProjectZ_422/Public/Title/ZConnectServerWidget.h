// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZConnectServerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZConnectServerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnMakeServerButtonClick();

	UFUNCTION()
	void OnConnectServerButtonClick();

protected:
	UPROPERTY()
	class UEditableTextBox* InputIP;

	UPROPERTY()
	class UButton* MakeServerButton;
	
	UPROPERTY()
	class UButton* ConnectServerButton;



};
