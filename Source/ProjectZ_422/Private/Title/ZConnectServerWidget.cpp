// Fill out your copyright notice in the Description page of Project Settings.


#include "ZConnectServerWidget.h"
#include "..\..\Public\Title\ZConnectServerWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ZGameInstance.h"
#include "Json.h"

void UZConnectServerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InputIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_InputIP")));
	check(InputIP);

	ErrorText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Error")));
	check(ErrorText);

	MakeServerButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_MakeServer")));
	check(MakeServerButton);

	ConnectServerButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_ConnectServer")));
	check(ConnectServerButton);

	MakeServerButton->OnClicked.AddDynamic(this, &UZConnectServerWidget::OnMakeServerButtonClick);
	ConnectServerButton->OnClicked.AddDynamic(this, &UZConnectServerWidget::OnConnectGameButtonClick);
}

void UZConnectServerWidget::OnMakeServerButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void UZConnectServerWidget::OnConnectGameButtonClick()
{
	auto IP = InputIP->GetText();
	if (IP.IsEmpty())
	{
		return;
	}	

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		FString URL = MyGameInstance->GetWebConnector().GetWebURL();
		URL.Append(TEXT("/match_game"));

		FString PostParam = FString::Printf(TEXT("ip=%s"), *IP.ToString());

		MyGameInstance->GetWebConnector().HttpPost(URL, PostParam, 
			FHttpRequestCompleteDelegate::CreateUObject(this, &UZConnectServerWidget::OnConnectGameResponse));
	}

}

void UZConnectServerWidget::OnConnectGameResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool bResult = JsonObject->GetBoolField(TEXT("result"));
			if (bResult)
			{
				FString ConnectIP = JsonObject->GetStringField(TEXT("ip"));
				UGameplayStatics::OpenLevel(GetWorld(), *ConnectIP);
			}
			else
			{
				FString Reason = JsonObject->GetStringField(TEXT("reason"));
				ErrorText->SetText(FText::FromString(Reason));
			}
		}
		else
		{
			ErrorText->SetText(FText::FromString(TEXT("데이터 수신 실패..")));
		}
	}
	else
	{
		ErrorText->SetText(FText::FromString(TEXT("서버 연결 실패..")));

	}
}
