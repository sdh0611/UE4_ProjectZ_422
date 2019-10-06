// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInputIDWidget.h"
#include "..\..\Public\Title\ZInputIDWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "ZTitleWidget.h"
#include "ZTitlePlayerController.h"

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

void UZInputIDWidget::CheckVerified() const
{
	/* 서버로 ID, PW보내서 결과 받아옴. */
	auto TitleWidget = Cast<UZTitleWidget>(GetParent());
	if (TitleWidget)
	{
		ZLOG(Error, TEXT("Parent."));
	}
	else
	{
		ZLOG(Error, TEXT("Parent fail."));
	}

	   
	auto MyPC = GetOwningPlayer<AZTitlePlayerController>();
	if (nullptr == MyPC)
	{
		ZLOG(Error, TEXT("Invalid player controller.."));
		return;

	}

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

	MyPC->ServerRequestLogin(TEXT("127.0.0.1:8000/login"), ID, PW);
	/* 일단 테스트용 */
	//bool bResult = true;
	//if (bResult)
	//{
	//	InputID->SetText(FText::GetEmpty());
	//	InputPW->SetText(FText::GetEmpty());
	//}

	//OnIdentify.Execute(bResult);

	//
}

void UZInputIDWidget::OnTextCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
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
