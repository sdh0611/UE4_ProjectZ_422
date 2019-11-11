// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZUserWidget.h"
#include "ZInGameMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZInGameMenuWidget : public UZUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void OnLeaveGame();


};
