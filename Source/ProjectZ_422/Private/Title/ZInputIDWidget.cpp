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

	//auto MyPC = GetOwningPlayer<AZTitlePlayerController>();
	//if (nullptr == MyPC)
	//{
	//	ZLOG(Error, TEXT("Invalid player controller.."));
	//	return;

	//}
	   
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

	//MyPC->ServerRequestLogin(TEXT("127.0.0.1:8000/login"), ID, PW);

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
		ZLOG(Error, TEXT("Invalid gameinstance.."));
		return;
	}

	MyGameInstance->HttpPostLogin(URL, UserID, UserPW, 
		FHttpRequestCompleteDelegate::CreateUObject(this, &UZInputIDWidget::OnLoginServerResponseReceived));

	
}

void UZInputIDWidget::OnLoginServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	bool bSuccess = false;
	FString Result;

	if (bWasSuccessful)
	{
		ZLOG_S(Error);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool bResult = JsonObject->GetBoolField("result");
			if (bResult)
			{
				FString NewUserID = JsonObject->GetStringField("id");
				FString NewNickname = JsonObject->GetStringField("nickname");
				ZLOG(Error, TEXT("Nickname : %s"), *NewNickname);	
				
				auto MyGameInstance = GetGameInstance<UZGameInstance>();
				if (nullptr == MyGameInstance)
				{
					ZLOG(Error, TEXT("Invalid gameinstance.."));
					return;
				}
				MyGameInstance->SetUserID(NewUserID);
				MyGameInstance->SetUserNickname(NewNickname);
				MyGameInstance->bIsVerified = true;

				bSuccess = true;
				Result = TEXT("로그인 성공!");
			}
			else
			{
				ZLOG(Error, TEXT("Result fail.."));
				Result = TEXT("일치하는 정보가 없습니다...");
			}
		}
		else
		{
			ZLOG(Error, TEXT("Deserialize fail.."));
			Result = TEXT("데이터 수신 실패...");
		}

	}
	else
	{
		Result = TEXT("서버 연결 실패...");
	}

	ReceiveLoginResponse(bSuccess, Result);


}

void UZInputIDWidget::ReceiveLoginResponse(bool bSuccess, const FString & ResultText)
{
	ZLOG_S(Error);
	SetErrorText(ResultText);

	OnIdentify.Execute(bSuccess);
}
