// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitleWidget.h"
#include "..\..\Public\Title\ZTitleWidget.h"
#include "Title/ZInputIDWidget.h"
#include "Title/ZConnectServerWidget.h"


void UZTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InputIDWidget = Cast<UZInputIDWidget>(GetWidgetFromName(TEXT("UI_ZInputIDWidget")));
	check(InputIDWidget);

	ConnectServerWidget = Cast<UZConnectServerWidget>(GetWidgetFromName(TEXT("UI_ZConnectServerWidget")));
	check(ConnectServerWidget);

	InputIDWidget->OnIdentify.BindUObject(this, &UZTitleWidget::SetIsVerified);


}

bool UZTitleWidget::IsVerified() const
{
	return bIsVerified;
}

void UZTitleWidget::SetIsVerified(bool bVerified)
{
	ZLOG_S(Error);
	bIsVerified = bVerified;
	if (bIsVerified)
	{
		InputIDWidget->SetVisibility(ESlateVisibility::Collapsed);
		ConnectServerWidget->SetVisibility(ESlateVisibility::Visible);
	}

}
