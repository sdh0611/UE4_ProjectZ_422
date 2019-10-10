// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInputIDWidget.h"
#include "..\..\Public\Title\ZInputIDWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "ZTitleWidget.h"
#include "ZTitlePlayerController.h"
#include "Json.h"
#include "ZGameInstance.h"

void UZInputIDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ErrorText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Error")));
	check(ErrorText);

	InputID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_InputID")));
	check(InputID);

	InputPW = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_InputPW")));
	check(InputPW);

	OKButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_OK")));
	check(OKButton);

	InputID->OnTextCommitted.AddDynamic(this, &UZInputIDWidget::OnTextCommit);
	InputPW->OnTextCommitted.AddDynamic(this, &UZInputIDWidget::OnTextCommit);

	OKButton->OnClicked.AddDynamic(this, &UZInputIDWidget::OnOKButtonClick);

}

void UZInputIDWidget::SetErrorText(const FString & NewText)
{
	ErrorText->SetText(FText::FromString(NewText));
}

void UZInputIDWidget::CheckVerified()
{
	/* 서버로 ID, PW보내서 결과 받아옴. */

	if (InputID->GetText().IsEmpty())
	{
		return;
	}

	if (InputPW->GetText().IsEmpty())
	{
		return;
	}

	FString ID = InputID->GetText().ToString();
	FString PW = InputPW->GetText().ToString();

	RequestLogin(TEXT("127.0.0.1:8000"), ID, PW);

}

void UZInputIDWidget::OnTextCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
	if (ETextCommit::OnEnter != CommitMethod)
	{
		return;
	}

	if (Text.IsEmpty())
	{
		return;
	}

	CheckVerified();

}

void UZInputIDWidget::OnOKButtonClick()
{
	CheckVerified();

}

void UZInputIDWidget::RequestLogin(const FString & URL, const FString & UserID, const FString & UserPW)
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (nullptr == MyGameInstance)
	{
		ZLOG(Error, TEXT("Invalid game instance.."));
		return;
	}

	MyGameInstance->GetWebConnector().Login(UserID, UserPW, 
		FOnLoginResponse::CreateUObject(this, &UZInputIDWidget::ReceiveLoginResponse));

}


void UZInputIDWidget::ReceiveLoginResponse(bool bSuccess, const FString & ResultText)
{
	SetErrorText(ResultText);

	OnIdentify.Execute(bSuccess);
}
