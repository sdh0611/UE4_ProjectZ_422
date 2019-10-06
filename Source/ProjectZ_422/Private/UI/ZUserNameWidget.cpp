// Fill out your copyright notice in the Description page of Project Settings.


#include "ZUserNameWidget.h"
#include "ZGameInstance.h"
#include "Components/TextBlock.h"
#include "ZBasePlayerController.h"
#include "Engine/World.h"

void UZUserNameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UserName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_UserName")));
	check(UserName);

}

void UZUserNameWidget::SetUserName(const FString & NewUserName)
{
	if (UserName)
	{
		ZLOG_S(Error);
		UserName->SetText(FText::FromString(NewUserName));
	}
}
