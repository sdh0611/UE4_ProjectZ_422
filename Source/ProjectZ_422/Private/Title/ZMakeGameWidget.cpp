// Fill out your copyright notice in the Description page of Project Settings.


#include "ZMakeGameWidget.h"
#include "ZClientGameInstance.h"
#include "ZTitlePlayerController.h"
#include "ZPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

void UZMakeGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SessionName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_SessionName")));
	check(SessionName);

	ConnectNumber = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_ConnectNumber")));
	check(ConnectNumber);

	OK = Cast<UButton>(GetWidgetFromName(TEXT("BTN_OK")));
	check(OK);

	OK->OnClicked.AddDynamic(this, &UZMakeGameWidget::OnCreateGameButtonClick);

}

void UZMakeGameWidget::OnCreateGameButtonClick()
{
	if (SessionName->GetText().IsEmpty())
	{
		/* ErrorText */

		return;
	}

	if (!ConnectNumber->GetText().IsNumeric())
	{
		/* ErrorText */

		return;
	}

	FString Num = ConnectNumber->GetText().ToString();
	if (Num.Contains(TEXT(".")))
	{
		/* ErrorText */

		return;
	}

	if (Num.Contains(TEXT("-")))
	{
		/* ErrorText */

		return;
	}
	
	FString Name = SessionName->GetText().ToString();

	auto MyGameInstance = GetGameInstance<UZClientGameInstance>();
	if (MyGameInstance)
	{
		MyGameInstance->CreateGameSession(Name, FCString::Atoi(*Num));
	}


}
