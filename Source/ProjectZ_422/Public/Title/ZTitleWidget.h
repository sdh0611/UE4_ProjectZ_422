// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	bool IsVerified() const;

private:
	void SetIsVerified(bool bVerified);

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsVerified = false;

protected:	

	UPROPERTY()
	class UZInputIDWidget* InputIDWidget;

	UPROPERTY()
	class UZConnectServerWidget* ConnectServerWidget;

};
