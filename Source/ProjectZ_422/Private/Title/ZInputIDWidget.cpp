// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInputIDWidget.h"
#include "..\..\Public\Title\ZInputIDWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Title/ZTitleWidget.h"

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
	/* ������ ID, PW������ ��� �޾ƿ�. */
	auto TitleWidget = Cast<UZTitleWidget>(GetParent());
	if (TitleWidget)
	{
		ZLOG(Error, TEXT("Parent."));
	}
	else
	{
		ZLOG(Error, TEXT("Parent fail."));
	}

	/* �ϴ� �׽�Ʈ�� */
	bool bResult = true;
	if (bResult)
	{
		InputID->SetText(FText::GetEmpty());
		InputPW->SetText(FText::GetEmpty());
	}

	OnIdentify.Execute(bResult);

	
}

void UZInputIDWidget::OnTextCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
	CheckVerified();

}

void UZInputIDWidget::OnOKButtonClick()
{
	CheckVerified();

}
