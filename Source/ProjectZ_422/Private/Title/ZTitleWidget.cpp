// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitleWidget.h"
#include "..\..\Public\Title\ZTitleWidget.h"
#include "Title/ZInputIDWidget.h"
#include "Title/ZConnectServerWidget.h"
#include "ZSessionListWidget.h"
#include "ZGameInstance.h"

void UZTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InputIDWidget = Cast<UZInputIDWidget>(GetWidgetFromName(TEXT("UI_ZInputIDWidget")));
	check(InputIDWidget);

	ConnectServerWidget = Cast<UZConnectServerWidget>(GetWidgetFromName(TEXT("UI_ZConnectServerWidget")));
	check(ConnectServerWidget);

	SessionListWidget = Cast<UZSessionListWidget>(GetWidgetFromName(TEXT("UI_ZSessionList")));
	check(SessionListWidget);

	InputIDWidget->OnIdentify.BindUObject(this, &UZTitleWidget::SetIsVerified);

}

void UZTitleWidget::SetIDInputWidgetErrorText(const FString & NewText)
{
	InputIDWidget->SetErrorText(NewText);
}

bool UZTitleWidget::IsVerified() const
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (nullptr == MyGameInstance)
	{
		return false;
	}

	return MyGameInstance->GetWebConnector().IsVerified();
}

void UZTitleWidget::SetIsVerified(bool bVerified)
{
	ZLOG_S(Error);
	//bIsVerified = bVerified;
	if (bVerified)
	{
		InputIDWidget->SetVisibility(ESlateVisibility::Collapsed);
		//ConnectServerWidget->SetVisibility(ESlateVisibility::Visible);
		SessionListWidget->SetVisibility(ESlateVisibility::Visible);
	}

}
