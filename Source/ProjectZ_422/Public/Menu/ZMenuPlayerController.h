// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZMenuPlayerController.generated.h"

/*
	NOTE(10.04) :
		Æó±â.
*/

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	//virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> MenuWidgetClass;

	UPROPERTY()
	class UUserWidget* MenuWidget;

};
