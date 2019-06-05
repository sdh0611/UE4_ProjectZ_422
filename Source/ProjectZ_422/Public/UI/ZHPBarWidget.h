// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZHPBarWidget.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


public:

	void BindStatus(class UZCharacterStatusComponent* NewStatusComponent);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = HPBar)
	void UpdateWidget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HPBar)
	class UZCharacterStatusComponent* StatusComponent;

};
