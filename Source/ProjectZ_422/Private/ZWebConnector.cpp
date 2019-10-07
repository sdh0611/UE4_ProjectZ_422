// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWebConnector.h"
#include "Json.h"
#include "ZGameInstance.h"

void UZWebConnector::PostInitProperties()
{
	Super::PostInitProperties();

	Http = &FHttpModule::Get();
}

void UZWebConnector::HttpPost(FString NewURL, const FString& PostParameter, FHttpRequestCompleteDelegate RequestDelegate)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UZWebConnector::OnLoginResponseReceive);

	Request->SetURL(NewURL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameter);
	Request->ProcessRequest();

}

void UZWebConnector::Login(const FString & NewUserID, const FString & NewUserPW, FOnLoginResponse LoginResponse)
{
	ZLOG(Error, TEXT("ID : %s, PW : %s"), *NewUserID, *NewUserPW);
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UZWebConnector::OnLoginResponseReceive);

	FString PostParameters = FString::Printf(TEXT("id=%s&password=%s"), *NewUserID, *NewUserPW);
	FString ConnectURL = URL;

	Request->SetURL(ConnectURL.Append(TEXT("/login")));
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();

	OnLoginResponse = LoginResponse;

}

void UZWebConnector::Logout()
{
	if (!bIsVerified)
	{
		return;
	}
	
	ZLOG(Error, TEXT("Logout ID : %s"), *UserID);
	
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UZWebConnector::OnLogoutResponseReceived);

	FString PostParameters = FString::Printf(TEXT("id=%s"), *UserID);
	FString ConnectURL = URL;

	Request->SetURL(ConnectURL.Append(TEXT("/logout")));
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();

}

void UZWebConnector::SetURL(const FString & NewURL)
{
	URL = NewURL;
}

bool UZWebConnector::IsVerified() const
{
	return bIsVerified;
}

const FString & UZWebConnector::GetWebURL() const
{
	return URL;
}

const FString & UZWebConnector::GetUserNickname() const
{
	return UserNickname;
}

void UZWebConnector::OnLoginResponseReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	bool bSuccess = false;
	FString Result;
	
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		ZLOG(Error, TEXT("%s"), *Response->GetContentAsString());
		
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool bResult = JsonObject->GetBoolField("result");
			if (bResult)
			{
				FString NewUserID = JsonObject->GetStringField("id");
				FString NewNickname = JsonObject->GetStringField("nickname");
				ZLOG(Error, TEXT("Nickname : %s"), *NewNickname);

				UserID = NewUserID;
				UserNickname = NewNickname;
				bIsVerified = true;

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
		ZLOG(Error, TEXT("Failed to send.."));
		Result = TEXT("서버 연결 실패...");
	}

	OnLoginResponse.Execute(bSuccess, Result);

}

void UZWebConnector::OnLogoutResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	ZLOG_S(Error);
	if (bWasSuccessful)
	{
		ZLOG_S(Error);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool bResult = JsonObject->GetBoolField("result");
			if (!bResult)
			{
				ZLOG(Error, TEXT("Logout fail.."));
				return;
			}

			UserID.Empty();
			UserNickname.Empty();
			bIsVerified = false;
		}
		else
		{
			ZLOG(Error, TEXT("Deserialize fail.."));
		}

	}

}
