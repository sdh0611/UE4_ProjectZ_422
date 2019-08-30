// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInputNumberWidget.h"
#include "ZNumberWidgetInterface.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

void UZInputNumberWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto NewInputText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ET_Input")));
	check(nullptr != NewInputText);
	InputText = NewInputText;

	auto NewOKButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_OK")));
	check(nullptr != NewOKButton);
	OKButton = NewOKButton;

	auto NewCancelButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Cancel")));
	check(nullptr != NewCancelButton);
	CancelButton = NewCancelButton;

	InputText->OnTextCommitted.AddDynamic(this, &UZInputNumberWidget::OnTextCommitted);
	OKButton->OnClicked.AddDynamic(this, &UZInputNumberWidget::OnOKButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UZInputNumberWidget::OnCancelButtonClicked);

	SetVisibility(ESlateVisibility::Hidden);
}

void UZInputNumberWidget::BindWidget(UUserWidget * NewWidget)
{
	WidgetBinded = NewWidget;
}

int32 UZInputNumberWidget::GetInputNumber() const
{
	return 0;
}

void UZInputNumberWidget::OnTextCommitted(const FText & Text, ETextCommit::Type CommitMethod)
{
	if (ETextCommit::OnEnter != CommitMethod)
	{
		return;
	}

	if (!Text.IsNumeric())
	{
		ZLOG(Error, TEXT("Not number."));
		return;
	}

	FString NumberString = Text.ToString();
	if (NumberString.Contains(TEXT(".")))
	{
		ZLOG(Error, TEXT("Not integer."));
		return;
	}

	if (NumberString.Contains(TEXT("-")))
	{
		ZLOG(Error, TEXT("Not positive."));
		return;
	}

	int Number = FCString::Atoi(*NumberString);
	ZLOG(Warning, TEXT("Commit number : %d"), Number);
	/* 숫자값을 넘겨줌. */
	if (WidgetBinded)
	{
		auto NumberInterface = Cast<IZNumberWidgetInterface>(WidgetBinded);
		if (NumberInterface)
		{
			NumberInterface->OnReceiveNumberInput(Number);
		}
	}

	/* 토글 */
	SetVisibility(ESlateVisibility::Hidden);
	WidgetBinded = nullptr;
}

void UZInputNumberWidget::OnOKButtonClicked()
{
	ZLOG_S(Warning);

	FText NumberText = InputText->GetText();

	if (!NumberText.IsNumeric())
	{
		ZLOG(Error, TEXT("Not number."));
		return;
	}

	FString NumberString = NumberText.ToString();
	if (NumberString.Contains(TEXT(".")))
	{
		ZLOG(Error, TEXT("Not integer."));
		return;
	}

	if (NumberString.Contains(TEXT("-")))
	{
		ZLOG(Error, TEXT("Not positive."));
		return;
	}

	int Number = FCString::Atoi(*NumberString);
	ZLOG(Warning, TEXT("Commit number : %d"), Number);
	/* 숫자값을 넘겨줌. */
	if (WidgetBinded)
	{
		auto NumberInterface = Cast<IZNumberWidgetInterface>(WidgetBinded);
		if (NumberInterface)
		{
			NumberInterface->OnReceiveNumberInput(Number);
		}
	}

	/* 토글 */
	SetVisibility(ESlateVisibility::Hidden);
	WidgetBinded = nullptr;
}

void UZInputNumberWidget::OnCancelButtonClicked()
{
	ZLOG_S(Warning);

	InputText->SetText(FText::GetEmpty());

	/* 토글 */
	SetVisibility(ESlateVisibility::Hidden);
	WidgetBinded = nullptr;
}
