// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZEndGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZEndGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
public:
	void SetEndText(const FString& NewText);

protected:
	UPROPERTY()
	class UTextBlock* EndText;

};
