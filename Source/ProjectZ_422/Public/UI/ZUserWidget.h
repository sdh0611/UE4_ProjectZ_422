// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void OnDrawScreen();

	UFUNCTION(BlueprintCallable)
	virtual void OnRemoveScreen();
	
public:
	bool bCanDestruct = false;


};
