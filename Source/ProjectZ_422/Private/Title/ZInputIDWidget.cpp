// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInputIDWidget.h"
#include "..\..\Public\Title\ZInputIDWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Title/ZTitleWidget.h"

void UZInputIDWidget::NativeConstruct()
{
	Super::NativeConstruct();

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

	/* 일단 테스트용 */
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
