// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZUserNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZUserNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	void SetUserName(const FString& NewUserName);

protected:
	UPROPERTY()
	class UTextBlock* UserName;

};
