// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "ZInputIDWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnIdentify, bool);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZInputIDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void SetErrorText(const FString& NewText);

protected:
	void CheckVerified();

	UFUNCTION()
	void OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnOKButtonClick();


public:
	FOnIdentify OnIdentify;

private:
	void RequestLogin(const FString& URL, const FString& UserID, const FString& UserPW);
	void ReceiveLoginResponse(bool bSuccess, const FString& ResultText);

protected:
	UPROPERTY()
	class UTextBlock* ErrorText;

	UPROPERTY()
	class UEditableTextBox* InputID;

	UPROPERTY()
	class UEditableTextBox* InputPW;

	UPROPERTY()
	class UButton* OKButton;

};
