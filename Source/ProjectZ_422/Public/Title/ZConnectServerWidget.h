// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "ZConnectServerWidget.generated.h"

/* 
	NOTE(11.01) : Æó±â¿¹Á¤ 
*/

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
	void OnConnectGameButtonClick();

	void OnConnectGameResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:
	UPROPERTY()
	class UEditableTextBox* InputIP;

	UPROPERTY()
	class UTextBlock* ErrorText;

	UPROPERTY()
	class UButton* MakeServerButton;
	
	UPROPERTY()
	class UButton* ConnectServerButton;



};
