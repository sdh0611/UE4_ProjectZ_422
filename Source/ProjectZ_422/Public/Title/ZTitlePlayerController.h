// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/PlayerController.h"
#include "ZTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> TitleWidgetClass;

	UPROPERTY()
	class UUserWidget* TitleWidget;


};
